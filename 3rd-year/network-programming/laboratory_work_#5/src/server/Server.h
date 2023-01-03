#pragma once

#include "../client_processing/ClientProcessing.h"
#include <algorithm>
#include <ares.h>
#include <map>
#include <stdexcept>

// state machine
enum SocketType {
  GET_AUTH_REQUEST,
  SEND_AUTH_RESPONSE,
  GET_CONNECT_REQUEST,
  SEND_CONNECT_RESPONSE,
  CONNECT,
  DATA_EXCHANGE_DEST,
  DATA_EXCHANGE_CLIENT,
  NONE
};

struct Socket {
  int socket_;
  SocketType type_;
  Socket(const int &socket, const SocketType &type)
      : socket_(socket), type_(type) {}
  explicit Socket(const int &socket) : socket_(socket), type_(NONE) {}
};

struct compare {
  bool operator()(const Socket &l, const Socket &r) const {
    return l.socket_ < r.socket_;
  }
};

class Server {
private:
  static Server *instance_;
  int server_socket_;
  ares_channel dns_channel_;

  std::map<Socket, std::shared_ptr<ClientProcessing>, compare> clients_;

  struct ares_arg {
    Server *instance;
    int socket_;
    ares_arg(Server *instance, const int socket)
        : instance(instance), socket_(socket) {}
  };

  static void ares_callback(void *arg, int status, int timeouts,
                            struct hostent *hostent);

  explicit Server(const in_port_t &port);

  int accept();

  bool addClientDestSocket(const int &client_socket, const SocketType &type);

  bool changeSocketType(const int &socket, const SocketType &new_type);

  std::shared_ptr<ClientProcessing> deleteClient(const int &socket);

public:
  static Server *getInstance(const in_port_t &port);

  // sockets tasks for select
  int getSocketsTasks(fd_set &w, fd_set &r, fd_set &e);

  [[nodiscard]] inline std::map<Socket, std::shared_ptr<ClientProcessing>,
                                compare>
  getClients() const {
    return clients_;
  }

  [[nodiscard]] inline int getServerSocket() const { return server_socket_; }

  [[nodiscard]] ares_channel getDnsChannel() const { return dns_channel_; }

  // execute client's action depends on a socket type
  int execClientAction(const int &socket, const bool &writable);

  ~Server() {
    close(server_socket_);
    ares_destroy(dns_channel_);
  }
};