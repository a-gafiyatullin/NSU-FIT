#include "NodeTable.h"

NodeTable::Message NodeTable::Message::NO_MESSAGE = { ParsedMessage::NO_PARSED_MESSAGE, nullptr, {}, 0 };
NodeTable*          NodeTable::_instance = nullptr;

NodeTable::NodeTable(const int &port)
  : _replace_tasks(SHA_BITS)
  , _table(SHA_BITS)
  , _stop(false) {

  _socket = socket(AF_INET, SOCK_DGRAM, 0);

  if(_socket < 0) {
    throw std::runtime_error("NodeTable::NodeTable: can't open a socket!");
  }

  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = LOOKUP_TIME;
  if(setsockopt(_socket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout) < 0) {
    throw std::runtime_error("NodeTable::NodeTable: can't set the socket to non-blocking mode!");
  }

  sockaddr_in svr_addr;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);

  if (bind(_socket, (sockaddr *) &svr_addr, sizeof(svr_addr)) < 0) {
    close(_socket);
    throw std::runtime_error("NodeTable::NodeTable: can't bind the socket!");
  }

  ulli t = time(nullptr);
  srand(time(nullptr));
  _self = std::make_shared<Node>(t, (sockaddr *)&svr_addr, sizeof (svr_addr),
                                 calc_hash(std::to_string(port) + std::to_string(t) + std::to_string(rand())));

  _service_thread = std::thread(&NodeTable::service_loop, this);
}

void NodeTable::service_loop() {
  while (!_stop) {
    _service_loop_lock.lock();

    while(!_msg_queue.empty()) {
      auto msg = _msg_queue.front();
      _msg_queue.pop();

      if(msg._msg._ans_type != ANS_UNKNOWN) {
        service_ans_msg(msg);
      } else {
        service_send_msg(msg);
      }
    }
    update_nodes();

    filtered_recv({}, {});

    _service_loop_lock.unlock();
  }
}

void NodeTable::service_ans_msg(const NodeTable::Message &msg) {

  switch (msg._msg._ans_type) {
  case ANS_PING:
    update_k_bucket(msg._msg._id, (sockaddr*)&msg._addr, msg._len);
    break;
  case ANS_FIND_NODE:
  case ANS_FIND_VALUE:
    update_k_bucket(msg._msg._nodes);
    break;
  }

  delete[] (char*)msg._buf;
}

void NodeTable::service_send_msg(const NodeTable::Message &msg) {

  switch (msg._msg._send_type) {
  case SEND_PING: {
    size_t size = 0;
    void *buf = MessageUtils::instance()->gen_ans_msg_to_node(size, {}, ANS_PING, _self->id());
    if(size > 0) {
      sendto(_socket, buf, size, 0, (sockaddr*)&msg._addr, msg._len);
      delete[] (char*)buf;
    }
    update_k_bucket(msg._msg._id, (sockaddr*)&msg._addr, msg._len);
    break;
  }
  case SEND_STORE: {
    uint32_t addr = (*(uint32_t*)msg._msg._buf);
    in_port_t port = (*(in_port_t*)((char*)msg._msg._buf + sizeof (uint32_t)));

    if(addr == 0) {
      addr = msg._addr.sin_addr.s_addr;
    }
    Storage::instance()->add_key_value(msg._msg._hash, addr, port);
    update_k_bucket(msg._msg._id, (sockaddr*)&msg._addr, msg._len);
    break;
  }
  case SEND_FIND_NODE: {
    size_t size = 0;
    auto &node_l = _table[k_bucket_order(msg._msg._hash)];
    void *buf = MessageUtils::instance()->gen_ans_msg_to_node(size, std::vector<std::shared_ptr<Node>>{node_l.begin(), node_l.end() }
                                                              , ANS_FIND_NODE, _self->id());
    if(size > 0) {
      sendto(_socket, buf, size, 0, (sockaddr*)&msg._addr, msg._len);
      delete[] (char*)buf;
    }
    update_k_bucket(msg._msg._id, (sockaddr*)&msg._addr, msg._len);
    break;
  }
  case SEND_FIND_VALUE: {
    auto info = Storage::instance()->get_value(msg._msg._hash);

    size_t size = 0;
    void *buf;
    if(info.first) {
      char *buffer = new char[sizeof (uint32_t) + sizeof (in_port_t)];
      memcpy(buffer, &info.second._addr, sizeof (uint32_t));
      memcpy(buffer + sizeof (uint32_t), &info.second._port, sizeof (in_port_t));

      buf = MessageUtils::instance()->gen_find_val_msg_to_node(size, buffer, sizeof (u_int32_t) + sizeof (in_port_t), _self->id());
    } else {
      auto &node_l = _table[k_bucket_order(msg._msg._hash)];
      buf = MessageUtils::instance()->gen_ans_msg_to_node(size, std::vector<std::shared_ptr<Node>>{node_l.begin(), node_l.end() }
                                                          , ANS_FIND_VALUE, _self->id());
    }

    if(size > 0) {
      sendto(_socket, buf, size, 0, (sockaddr*)&msg._addr, msg._len);
      delete[] (char*)buf;
    }
  }
  }
}

NodeTable *NodeTable::init(const int &port) {
  return (_instance == nullptr ? (_instance = new NodeTable(port)) : _instance);
}

NodeTable *NodeTable::instance() {
  return _instance;
}

void NodeTable::store(const value_hash &hash, const void *buf, const size_t &size) {
  auto nodes = node_lookup(hash);

  for(auto node = nodes.begin(); node != nodes.end(); node++) {
    (*node)->store(hash, buf, size, _socket, _self->id());
  }
}

bool NodeTable::value_lookup(const value_hash &hash) {
  _service_loop_lock.lock();

  int order = k_bucket_order(hash);

  int start_order = order;
  std::list<std::shared_ptr<Node>> nearest_nodes;

  int required_nodes_num = 0;
  do {
    for(auto node = _table[order].begin(); node != _table[order].end() && required_nodes_num != K_BUCKET_SIZE; node++) {
      required_nodes_num++;
      nearest_nodes.push_back(*node);
    }

    order++;
  } while (required_nodes_num != K_BUCKET_SIZE || start_order == order);

  nearest_nodes.sort([hash](const std::shared_ptr<Node> &l, const std::shared_ptr<Node> &r) {return l->id() - hash < r->id() - hash; });

  bool found = true;
  ulli curr_time = time(nullptr);
  while(time(nullptr) - curr_time < LOOKUP_TIME) {
    int lookup_start = 0;
    int lookup_end = ALPHA;

    if(!found) {
      lookup_start = ALPHA;
      lookup_end = nearest_nodes.size();
      found = true;
    }

    for(auto node = nearest_nodes.begin(); lookup_start < lookup_end; node++, lookup_start++) {
      (*node)->find_value(hash, _socket, _self->id());
    }

    Message msg = filtered_recv({ANS_FIND_VALUE}, {});

    if(Message::is_no_message(msg)) {
      continue;
    }

    if(msg._msg._buf_size != 0 && msg._msg._buf != nullptr) {
      uint32_t addr = (*(uint32_t*)msg._msg._buf);
      in_port_t port = (*(in_port_t*)((char*)msg._msg._buf + sizeof (uint32_t)));

      if(addr == 0) {
        addr = msg._addr.sin_addr.s_addr;
      }
      Storage::instance()->add_key_value(msg._msg._hash, addr, port);
      update_k_bucket(msg._msg._id, (sockaddr*)&msg._addr, msg._len);

      delete[] (char*)msg._buf;
      _service_loop_lock.unlock();

      return true;
    }

    found = conditional_update(nearest_nodes, msg._msg._nodes, hash);
    update_k_bucket(msg._msg._nodes);

    delete[] (char*)msg._buf;
  }

  _service_loop_lock.unlock();
  return false;
}

void NodeTable::release() {
  _instance->_stop = true;
  _instance->_service_thread.join();

  delete _instance;
  _instance = nullptr;
}

std::list<std::shared_ptr<Node>> NodeTable::node_lookup(const node_id &id) {
  _service_loop_lock.lock();

  int order = k_bucket_order(id);

  int start_order = order;
  std::list<std::shared_ptr<Node>> nearest_nodes;

  int required_nodes_num = 0;
  do {
    for(auto node = _table[order].begin(); node != _table[order].end() && required_nodes_num != K_BUCKET_SIZE; node++) {
      required_nodes_num++;
      nearest_nodes.push_back(*node);
    }

    order++;
  } while (required_nodes_num != K_BUCKET_SIZE || start_order == order);

  nearest_nodes.sort([id](const std::shared_ptr<Node> &l, const std::shared_ptr<Node> &r) {return l->id() - id < r->id() - id; });

  bool found = true;
  ulli curr_time = time(nullptr);
  while(time(nullptr) - curr_time < LOOKUP_TIME) {
    int lookup_start = 0;
    int lookup_end = ALPHA;

    if(!found) {
      lookup_start = ALPHA;
      lookup_end = nearest_nodes.size();
      found = true;
    }

    for(auto node = nearest_nodes.begin(); lookup_start < lookup_end; node++, lookup_start++) {
      (*node)->find_node(id, _socket, _self->id());
    }

    Message msg = filtered_recv({ANS_FIND_NODE}, {});

    if(Message::is_no_message(msg)) {
      continue;
    }

    found = conditional_update(nearest_nodes, msg._msg._nodes, id);
    update_k_bucket(msg._msg._nodes);

    delete[] (char*)msg._buf;
  }

  _service_loop_lock.unlock();
  return nearest_nodes;
}

int NodeTable::k_bucket_order(const node_id &id) const {
  size_t i = 0;
  unsigned char mask = ~0;
  for(; i < id.size(); i++) {
    if((id[i] & mask) != 0) {
      break;
    }
  }

  int order = (20 - (i + 1)) * 8;
  mask = 1;
  int j = 0;
  for(; j < 8; j++) {
    if((id[i] & (mask << j)) != 0) {
      break;
    }
  }

  order += j;

  return order;
}

void NodeTable::update_k_bucket(const node_id &id, const sockaddr *addr, const socklen_t &len) {
  int order = k_bucket_order(id);

  for(auto node = _table[order].begin(); node != _table[order].end(); node++) {
    if((*node)->is_your_id(id)) {
      auto c_node = *node;
      c_node->update_time(time(nullptr));
      _table[order].erase(node);
      _table[order].push_back(c_node);
      return;
    }
  }

  auto node = std::make_shared<Node>(time(nullptr), addr, len, id);

  if(_table[order].size() < K_BUCKET_SIZE) {
    _table[order].push_back(node);
  } else {
    _table[order].front()->ping(_socket, _self->id());
    _replace_tasks[order] = { node, (ulli)time(nullptr) };
  }
}

void NodeTable::update_k_bucket(const std::vector<std::shared_ptr<Node> > &nodes) {
  for(size_t i = 0; i < nodes.size(); i++) {
    int order = k_bucket_order(nodes[i]->id());

    for(auto node = _table[order].begin(); node != _table[order].end(); node++) {
      if((*node)->is_your_id(nodes[i]->id())) {
        auto c_node = *node;
        c_node->update_time(time(nullptr));
        _table[order].erase(node);
        _table[order].push_back(c_node);
        return;
      }
    }

    if(_table[order].size() < K_BUCKET_SIZE) {
      _table[order].push_back(nodes[i]);
    } else {
      _table[order].front()->ping(_socket, _self->id());
      _replace_tasks[order] = { nodes[i], (ulli)time(nullptr) };
    }
  }
}

void NodeTable::update_nodes() {
  for(size_t i = 0; i < _replace_tasks.size(); i++) {
    if(_replace_tasks[i]._node_for_replace != nullptr && (time(nullptr) - _replace_tasks[i]._time) > DELAY_FOR_REPLACEMENT) {
      if(_table[i].front()->time() < _replace_tasks[i]._time) {
        _table[i].erase(_table[i].begin());
        _table[i].push_back(_replace_tasks[i]._node_for_replace);
      } else {
        _replace_tasks[i]._node_for_replace = nullptr;
      }
    }
  }
}

bool NodeTable::conditional_update(std::list<std::shared_ptr<Node> > &nearest_nodes
                                   , const std::vector<std::shared_ptr<Node> > &possible_nodes
                                   , const value_hash_or_node_id &pivot) {
  bool updated = false;

  for(size_t i = 0; i < possible_nodes.size(); i++) {
    if(possible_nodes[i]->id() - pivot < nearest_nodes.front()->id() - pivot) {
      updated = true;
      nearest_nodes.push_front(possible_nodes[i]);
      nearest_nodes.pop_back();
    }
  }

  return updated;
}

NodeTable::Message NodeTable::filtered_recv(const std::vector<ANS_MSG_TYPES> &desired_ans
                                     , const std::vector<SEND_MSG_TYPES> &desired_sends) {

  Message msg;
  msg._buf = new char[BUFF_SIZE];

  ssize_t code = recvfrom(_socket, msg._buf, BUFF_SIZE, 0, (sockaddr*)&msg._addr, &msg._len);
  if(code < 0) {
    delete[] (char*)msg._buf;
    return Message::NO_MESSAGE;
  }

  msg._msg = MessageUtils::instance()->parse_recved_msg(msg._buf, BUFF_SIZE);
  if(ParsedMessage::is_no_parsed_msg(msg._msg)) {
    return Message::NO_MESSAGE;
  }

  if(msg._msg._ans_type != ANS_UNKNOWN) {
    for(size_t i = 0; i < desired_ans.size(); i++) {
      if(msg._msg._ans_type == desired_ans[i]) {
        return msg;
      }
    }
  } else {
    for(size_t i = 0; i < desired_ans.size(); i++) {
      if(msg._msg._send_type == desired_sends[i]) {
        return msg;
      }
    }
  }

  _msg_queue.push(msg);
  return Message::NO_MESSAGE;
}

bool NodeTable::Message::is_no_message(const NodeTable::Message &msg) {
  return ParsedMessage::is_no_parsed_msg(msg._msg);
}
