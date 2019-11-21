#include "Node.h"

Node::Node(const std::string &name, const int &loss_rate,
           const in_port_t &local_port)
    : Node(name, loss_rate, local_port, "127.0.0.1", local_port) {}

Node::Node(std::string name, const int &loss_rate, const in_port_t &local_port,
           const std::string &parent_ip, const in_port_t &parent_port)
    : name(std::move(name)), loss_rate(loss_rate),
      local_socket(socket(AF_INET, SOCK_DGRAM, 0)),
      repeat_exchange_thread([this] { repeatExchangeMessages(); }) {

  local_address.setAddress("127.0.0.1", local_port);

  parent_address.setAddress(parent_ip, parent_port);

  srand(time(nullptr));
}

Node::~Node() { close(local_socket); }

int Node::exchangeMessages() {
  if (bind(local_socket, local_address.getSockaddrStruct(),
           SOCKADDR_STRUCT_SIZE) < 0) {
    std::perror("bind");
    return errno;
  }
  sendCommandToNode("REQCONN:", parent_address.getSockaddrStruct(),
                    SOCKADDR_STRUCT_SIZE, parent_address);

  char buffer[BUFSIZ];
  fd_set fdset;
  FD_ZERO(&fdset);
  FD_SET(STDIN_FILENO, &fdset);
  FD_SET(local_socket, &fdset);
  IPv4Address remote_node_parent;

  while (select(local_socket + 1, &fdset, nullptr, nullptr, nullptr) != -1) {
    if (FD_ISSET(local_socket, &fdset)) {
      IPv4Address answer_address;
      socklen_t answer_address_len = SOCKADDR_STRUCT_SIZE;
      size_t recv_len =
          recvfrom(local_socket, buffer, BUFSIZ, 0,
                   answer_address.getSockaddrStruct(), &answer_address_len);
      answer_address.update();
      buffer[recv_len] = '\0';

      auto command = std::string(buffer).substr(0, COMMAND_LEN);
      if (command == "REQCONN:") {
        registerNewChildNode(buffer, answer_address);
      } else if (command == "ANSCONN:") {
        memcpy(remote_node_parent.getSockaddrStruct(), buffer + COMMAND_LEN,
               SOCKADDR_STRUCT_SIZE);
        remote_node_parent.update();
        neighbours_nodes_mutex.lock();
        neighbours_nodes.insert(
            std::make_pair(parent_address, remote_node_parent));
        neighbours_nodes_mutex.unlock();
      } else if (command == "UPDPRNT:") {
        memcpy(remote_node_parent.getSockaddrStruct(), buffer + COMMAND_LEN,
               SOCKADDR_STRUCT_SIZE);
        remote_node_parent.update();
        neighbours_nodes_mutex.lock();
        neighbours_nodes[answer_address] = remote_node_parent;
        neighbours_nodes_mutex.unlock();
      } else if (command == "CONFIRM:") {
        boost::uuids::uuid uuid{};
        memcpy(uuid.data, buffer + COMMAND_LEN, uuid.size());
        sent_msgs_mutex.lock();
        sent_msgs.erase(uuid);
        sent_msgs_mutex.unlock();
      } else {
        auto msg = readMsg(buffer, answer_address);
        if (msg.has_value()) {
          auto message_rez = parseMsg(msg.value().second);
          std::cout << message_rez.first << ": " << message_rez.second;
          sendMsgToNeighbours(msg.value().second, answer_address);
        }
      }
    }
    if (FD_ISSET(STDIN_FILENO, &fdset)) {
      buffer[0] = name.length();
      memcpy(buffer + 1, name.c_str(), buffer[0]);
      size_t length =
          read(STDIN_FILENO, buffer + buffer[0] + 1, BUFSIZ * sizeof(char));
      if (length <= 0) {
        continue;
      }

      buffer[length + buffer[0] + 1] = '\0';
      sendMsgToNeighbours(std::string(buffer), local_address);
    }
    FD_ZERO(&fdset);
    FD_SET(STDIN_FILENO, &fdset);
    FD_SET(local_socket, &fdset);
  }
  return 0;
}

void Node::repeatExchangeMessages() {
  while (1) {
    sent_msgs_mutex.lock();
    auto local_sent_msgs = sent_msgs;
    sent_msgs_mutex.unlock();
    for (auto &msg_info : local_sent_msgs) {
      time_t curr_time = time(nullptr);
      if (curr_time - msg_info.second.getTime() >= SEND_TIMEOUT &&
          msg_info.second.getTries() <= TRIES) {
        resendMsgToNode(msg_info.first, msg_info.second);
        sent_msgs_mutex.lock();
        sent_msgs[msg_info.first].updateTime();
        sent_msgs_mutex.unlock();
      } else if (msg_info.second.getTries() > TRIES) {
        neighbours_nodes_mutex.lock();
        auto child = neighbours_nodes.find(msg_info.second.getRecver());
        neighbours_nodes_mutex.unlock();
        if (child != neighbours_nodes.end() && child->second != local_address) {
          neighbours_nodes_mutex.lock();
          neighbours_nodes.erase(child->first);
          neighbours_nodes_mutex.unlock();
          sendCommandToNode("REQCONN:", parent_address.getSockaddrStruct(),
                            SOCKADDR_STRUCT_SIZE, child->second);
          if (child->first == parent_address) {
            parent_address = child->second;
            sendCommandToNeighbours(
                "UPDPRNT:", parent_address.getSockaddrStruct(),
                SOCKADDR_STRUCT_SIZE, parent_address);
          }
          sent_msgs_mutex.lock();
          sent_msgs[msg_info.first].setNewRecver(child->second);
          sent_msgs_mutex.unlock();
        } else {
          sent_msgs_mutex.lock();
          sent_msgs.erase(msg_info.first);
          sent_msgs_mutex.unlock();
        }
      }
    }
  }
}

void Node::registerNewChildNode(const char *info,
                                const IPv4Address &answer_address) {
  IPv4Address remote_node_parent;
  memcpy(remote_node_parent.getSockaddrStruct(), info + COMMAND_LEN,
         SOCKADDR_STRUCT_SIZE);
  remote_node_parent.update();
  neighbours_nodes_mutex.lock();
  neighbours_nodes.insert(std::make_pair(answer_address, remote_node_parent));
  neighbours_nodes_mutex.unlock();

  sendCommandToNode("ANSCONN:", parent_address.getSockaddrStruct(),
                    SOCKADDR_STRUCT_SIZE, answer_address);
  if (parent_address == local_address) {
    parent_address = answer_address;
    sendCommandToNeighbours("UPDPRNT:", answer_address.getSockaddrStruct(),
                            SOCKADDR_STRUCT_SIZE, answer_address);
  } else if (neighbours_nodes.size() == 2) {
    sendCommandToNeighbours("UPDPRNT:", answer_address.getSockaddrStruct(),
                            SOCKADDR_STRUCT_SIZE, answer_address);
  }
}

std::optional<std::pair<boost::uuids::uuid, std::string>>
Node::readMsg(const char *msg, const IPv4Address &sender) {

  boost::uuids::uuid uuid{};
  memcpy(uuid.data, msg, uuid.size());
  std::string text(msg + uuid.size());

  sendCommandToNode("CONFIRM:", uuid.data, uuid.size(), sender);
  auto prev_info = confirmed_recved_msgs.find(uuid);
  if (prev_info != confirmed_recved_msgs.end()) {
    prev_info->second = time(nullptr);
    return {};
  } else {
    confirmed_recved_msgs.insert(std::make_pair(uuid, time(nullptr)));
    return std::make_pair(uuid, text);
  }
}

void Node::sendMsgToNeighbours(const std::string &msg,
                               const IPv4Address &sender) {
  neighbours_nodes_mutex.lock();
  auto tmp_neighbours_nodes = neighbours_nodes;
  neighbours_nodes_mutex.unlock();
  for (auto &iter : tmp_neighbours_nodes) {
    if (iter.first != sender && rand() % 100 >= loss_rate) {
      sendMsgToNode(msg, iter.first);
    }
  }
}

void Node::sendMsgToNode(const std::string &msg, const IPv4Address &recver) {
  if (recver == local_address) {
    return;
  }
  boost::uuids::uuid uuid = uuid_generator();

  char buffer[BUFSIZ];
  memcpy(buffer, uuid.data, uuid.size());
  memcpy(buffer + uuid.size(), msg.c_str(), msg.size());
  sendto(local_socket, buffer, uuid.size() + msg.size(), 0,
         recver.getSockaddrStruct(), SOCKADDR_STRUCT_SIZE);
  sent_msgs_mutex.lock();
  sent_msgs.insert(std::make_pair(uuid, MsgInfo(msg, recver)));
  sent_msgs_mutex.unlock();
}

std::pair<std::string, std::string> Node::parseMsg(const std::string &msg) {
  return std::make_pair(msg.substr(1, msg.at(0)), msg.substr(msg.at(0) + 1));
}

void Node::resendMsgToNode(const boost::uuids::uuid &uuid,
                           const MsgInfo &msg_info) {
  char buffer[BUFSIZ];
  memcpy(buffer, uuid.data, uuid.size());
  memcpy(buffer + uuid.size(), msg_info.getText().c_str(),
         msg_info.getText().size());
  sendto(local_socket, buffer, uuid.size() + msg_info.getText().size(), 0,
         msg_info.getRecver().getSockaddrStruct(), SOCKADDR_STRUCT_SIZE);
}

void Node::sendCommandToNode(const std::string &command, const void *data,
                             size_t len, const IPv4Address &address) const {
  if (address == local_address) {
    return;
  }
  char buffer[BUFSIZ];
  memcpy(buffer, command.c_str(), COMMAND_LEN);
  if (len != 0 && data != nullptr) {
    memcpy(buffer + COMMAND_LEN, data, len);
  }
  sendto(local_socket, buffer, COMMAND_LEN + len, 0,
         address.getSockaddrStruct(), SOCKADDR_STRUCT_SIZE);
}

void Node::sendCommandToNeighbours(const std::string &command, const void *data,
                                   size_t len, const IPv4Address &sender) {
  neighbours_nodes_mutex.lock();
  auto tmp_neighbours_nodes = neighbours_nodes;
  neighbours_nodes_mutex.unlock();
  for (auto &iter : tmp_neighbours_nodes) {
    if (iter.first != sender) {
      sendCommandToNode(command, data, len, iter.first);
    }
  }
}

MsgInfo::MsgInfo(std::string text, const IPv4Address &recver)
    : text(std::move(text)), recver(recver), tries(0) {
  sending_time = time(nullptr);
}

void MsgInfo::setNewRecver(const IPv4Address &rcvr) {
  tries = 0;
  this->recver = rcvr;
}

void MsgInfo::updateTime() {
  sending_time = time(nullptr);
  tries++;
}
