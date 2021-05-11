#include "Node.h"

void Node::update_time(const ulli &time) {
  _time = time;
}

bool Node::send_helper(const value_hash_or_node_id &data, int socket, const SEND_MSG_TYPES &type, const node_id &from) {
  char *msg = nullptr;
  size_t size = -1;

  msg = (char*)MessageUtils::instance()->gen_send_msg_to_node(size, data, type, from);

  if(msg == nullptr || size <= 0) {
    delete msg;
    return false;
  }

  if(sendto(socket, msg, size, 0, (sockaddr*)&_addr, _socklen) <= 0) {
    return false;
  }

  delete msg;

  return true;
}

Node::Node(const ulli &time, const sockaddr *addr, const socklen_t &socklen, const node_id& id)
  : _time(time)
  , _socklen(socklen) {

  memcpy(&_addr, addr, socklen);
  _id = id;
}

bool Node::find_node(const node_id& id, int socket, const node_id &from) {
  return send_helper(id, socket, SEND_FIND_NODE, from);
}

bool Node::find_value(const value_hash& hash, int socket, const node_id &from) {
  return send_helper(hash, socket, SEND_FIND_VALUE, from);
}

bool Node::store(const value_hash& hash, const void *data, const size_t &data_size, int socket, const node_id &from){
  char *msg = nullptr;
  size_t size = -1;

  msg = (char*)MessageUtils::instance()->gen_str_msg_to_node(size, hash, data, data_size, from);

  if(msg == nullptr || size <= 0) {
    delete msg;
    return false;
  }

  if(sendto(socket, msg, size, 0, (sockaddr*)&_addr, _socklen) <= 0) {
    return false;
  }

  delete msg;

  return true;
}

bool Node::ping(int socket, const node_id &from) {
  return send_helper({}, socket, SEND_PING, from);
}

value_hash Node::distance(const value_hash &hash) {
  value_hash answer(SHA_DIGEST_LENGTH);

  for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
    answer[i] = _id[i] ^ hash[i];
  }

  return answer;
}

value_hash Node::distance(const Node &node) {
  return distance(node._id);
}

bool Node::is_your_id(const node_id &id) {
  return _id == id;
}
