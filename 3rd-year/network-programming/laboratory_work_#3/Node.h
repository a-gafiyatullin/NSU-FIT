#pragma once

#include "IPv4Address.h"
#include <arpa/inet.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <ctime>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <netinet/in.h>
#include <optional>
#include <string>
#include <sys/select.h>
#include <thread>
#include <unistd.h>
#include <utility>

class MsgInfo {
private:
  std::string text;
  time_t sending_time{};
  IPv4Address recver;
  int tries{};

public:
  MsgInfo(std::string text, const IPv4Address &recver);
  MsgInfo() = default;

  void updateTime();
  [[nodiscard]] inline time_t getTime() const { return sending_time; }
  [[nodiscard]] inline std::string getText() const { return text; }
  [[nodiscard]] inline IPv4Address getRecver() const { return recver; }
  [[nodiscard]] inline int getTries() const { return tries; }
  void setNewRecver(const IPv4Address &recver);
};

class Node {
private:
  std::string name;
  int loss_rate;

  IPv4Address local_address;
  IPv4Address parent_address;

  std::thread repeat_exchange_thread;
  int local_socket;

  boost::uuids::random_generator uuid_generator;
  std::mutex sent_msgs_mutex;
  std::map<boost::uuids::uuid, MsgInfo> sent_msgs;
  std::mutex neighbours_nodes_mutex;
  std::map<IPv4Address, IPv4Address> neighbours_nodes;
  std::map<boost::uuids::uuid, time_t> confirmed_recved_msgs;

  static const size_t COMMAND_LEN = strlen("REQCONN:");
  static const size_t SOCKADDR_STRUCT_SIZE = sizeof(sockaddr_in);
  static const int SEND_TIMEOUT = 2; // sec;
  static const int TRIES = 3;

  void registerNewChildNode(const char *info,
                            const IPv4Address &answer_address);

  void sendMsgToNode(const std::string &msg, const IPv4Address &recver);

  void resendMsgToNode(const boost::uuids::uuid &uuid, const MsgInfo &msg_info);

  void sendCommandToNode(const std::string &command, const void *data,
                         size_t len, const IPv4Address &address) const;

  void sendMsgToNeighbours(const std::string &msg, const IPv4Address &sender);

  void sendCommandToNeighbours(const std::string &command, const void *data,
                               size_t len, const IPv4Address &sender);

  std::optional<std::pair<boost::uuids::uuid, std::string>>
  readMsg(const char *msg, const IPv4Address &sender);

  static std::pair<std::string, std::string> parseMsg(const std::string &msg);

  void repeatExchangeMessages();

public:
  Node(const std::string &name, const int &loss_rate,
       const in_port_t &local_port);

  Node(std::string name, const int &loss_rate, const in_port_t &local_port,
       const std::string &parent_ip, const in_port_t &parent_port);

  int exchangeMessages();

  ~Node();
};
