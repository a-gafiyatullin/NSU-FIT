#pragma once

#include "../utility/SocksRequestParser.h"
#include "../utility/network_utility.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <memory>
#include <netdb.h>
#include <netinet/in.h>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>

class ClientProcessing {
private:
  // connection info
  int client_socket_;
  int dest_socket_;
  sockaddr_in dest_address_;
  bool start_to_connect_;
  int connection_error_;
  char *connection_response_;

  // resource info
  std::shared_ptr<SocksRequestParser> socks_header_;
  std::queue<std::pair<char *, ssize_t>> client_data_;
  std::queue<std::pair<char *, ssize_t>> dest_data_;
  // pos in the data piece from previous iteration
  ssize_t client_buffer_pos_;
  ssize_t client_buffer_length_;
  ssize_t dest_buffer_pos_;
  ssize_t dest_buffer_length_;

public:
  explicit ClientProcessing(const int &socket);

  int readAuthHeader();

  int sendAuthResponse();

  int readConnectHeader();

  int sendConnectResponse();

  inline void setConnectResponseStatus(const int &status) {
    connection_error_ = status;
  }

  [[nodiscard]] inline int getParsedAddressType() const {
    return socks_header_->getAddressType();
  }

  [[nodiscard]] inline std::string getParsedNameAddress() const {
    return socks_header_->getNameAddress().value();
  }

  [[nodiscard]] inline in_port_t getParsedPort() const {
    return socks_header_->getPort();
  }

  [[nodiscard]] inline in_addr getParsedIPv4Address() const {
    return socks_header_->getIPv4Address().value();
  }

  int exchangeClientData(const bool &write);

  int exchangeDestData(const bool &write);

  [[nodiscard]] inline int getDestSocket() const { return dest_socket_; }

  [[nodiscard]] inline int getSocket() const { return client_socket_; }

  int connectDestHost();

  void setDestAddress(const in_addr &address, const in_port_t &port);

  ~ClientProcessing();
};