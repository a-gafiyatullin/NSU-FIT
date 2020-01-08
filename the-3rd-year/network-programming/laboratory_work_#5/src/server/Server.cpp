#include "Server.h"

Server *Server::instance_ = nullptr;

Server::Server(const in_port_t &port)
    : server_socket_(socket(AF_INET, SOCK_STREAM, 0)) {
  if (server_socket_ < 0) {
    throw std::out_of_range("Server::Server : socket < 0!");
  }
  if (port < 1024) {
    throw std::out_of_range("Server::Server : port < 1024!");
  }

  sockaddr_in address{};
  address.sin_family = AF_INET;
  address.sin_port = htons(port);
  address.sin_addr.s_addr = INADDR_ANY;
  if (bind(server_socket_, (sockaddr *)&address, sizeof(sockaddr_in)) < 0) {
    throw std::domain_error("Server::Server : socket cannot bind port!");
  }
  if (::listen(server_socket_, SOMAXCONN) < 0) {
    throw std::domain_error("Server::Server : socket cannot listen socket!");
  }
  if (ares_library_init(ARES_LIB_INIT_ALL) != ARES_SUCCESS) {
    throw std::domain_error("Server::Server : cannot init dns resolver!");
  }
  if (ares_init(&dns_channel_) != ARES_SUCCESS) {
    throw std::domain_error("Server::Server : cannot init dns resolver!");
  }
}

Server *Server::getInstance(const in_port_t &port) {
  if (instance_ == nullptr) {
    instance_ = new Server(port);
  }

  return instance_;
}

int Server::accept() {
  sockaddr_in remote{};
  socklen_t sockaddr_in_len = sizeof(sockaddr_in);
  int client_socket;
  if ((client_socket = ::accept(server_socket_, (sockaddr *)&remote,
                                &sockaddr_in_len)) < 0) {
    return client_socket;
  }

  clients_.insert(
      std::make_pair(Socket(client_socket, GET_AUTH_REQUEST),
                     std::make_shared<ClientProcessing>(client_socket)));

  return client_socket;
}

std::shared_ptr<ClientProcessing> Server::deleteClient(const int &socket) {
  auto client = clients_.find(Socket(socket));
  if (client == clients_.end()) {
    return nullptr;
  }

  clients_.erase(client);
  return client->second;
}

bool Server::addClientDestSocket(const int &socket, const SocketType &type) {
  auto client = clients_.find(Socket(socket));
  if (client == clients_.end()) {
    return false;
  }
  clients_.insert(std::make_pair(Socket(client->second->getDestSocket(), NONE),
                                 client->second));

  return true;
}

int Server::getSocketsTasks(fd_set &w, fd_set &r, fd_set &e) {
  FD_ZERO(&w);
  FD_ZERO(&r);
  FD_ZERO(&e);

  int max_socket = ares_fds(dns_channel_, &r, &w) - 1;

  for (auto client = clients_.begin(); client != clients_.end(); client++) {
    switch (client->first.type_) {
    case GET_AUTH_REQUEST:
    case GET_CONNECT_REQUEST:
      FD_SET(client->first.socket_, &r);
      break;
    case CONNECT:
    case SEND_CONNECT_RESPONSE:
    case SEND_AUTH_RESPONSE:
      FD_SET(client->first.socket_, &w);
      break;
    case DATA_EXCHANGE_DEST:
    case DATA_EXCHANGE_CLIENT:
      FD_SET(client->first.socket_, &r);
      FD_SET(client->first.socket_, &w);
      break;
    case NONE:
      break;
    }
    if (client->first.socket_ > max_socket) {
      max_socket = client->first.socket_;
    }
  }
  FD_SET(server_socket_, &r);
  if (server_socket_ > max_socket) {
    max_socket = server_socket_;
  }

  return max_socket;
}

int Server::execClientAction(const int &socket, const bool &writable) {
  if (socket == server_socket_) {
#ifdef DEBUG
    std::cerr << "New connection!" << std::endl;
#endif
    return accept();
  }
  auto client = clients_.find(Socket(socket));
  if (client == clients_.end()) {
    return -1;
  }
  switch (client->first.type_) {
  case GET_AUTH_REQUEST: {
    int status = client->second->readAuthHeader();
    if (status == -1) {
#ifdef DEBUG
      std::cerr << "GET_AUTH_REQUEST: Connection is rejected!" << std::endl;
#endif
      deleteClient(socket);
      return -1;
    } else if (status > 0) {
      changeSocketType(socket, SEND_AUTH_RESPONSE);
    }
    break;
  }
  case SEND_AUTH_RESPONSE: {
    int status = client->second->sendAuthResponse();
    if (status == -1) {
#ifdef DEBUG
      std::cerr << "SEND_AUTH_RESPONSE: Connection is rejected!" << std::endl;
#endif
      deleteClient(socket);
      return -1;
    } else if (status > 0) {
      changeSocketType(socket, GET_CONNECT_REQUEST);
    }
    break;
  }
  case GET_CONNECT_REQUEST: {
    int status = client->second->readConnectHeader();
    if (status < 0) {
#ifdef DEBUG
      std::cerr << "GET_CONNECT_REQUEST: Connection is rejected!" << std::endl;
#endif
      if (status != -1) {
        changeSocketType(socket, SEND_CONNECT_RESPONSE);
      } else {
        deleteClient(socket);
        return -1;
      }
    } else if (status > 0) {
      addClientDestSocket(socket, NONE);
      changeSocketType(socket, NONE);
      switch (client->second->getParsedAddressType()) {
      case SocksRequestParser::IPv4_ATYP:
        client->second->setDestAddress(
            (in_addr)client->second->getParsedIPv4Address(),
            client->second->getParsedPort());
        changeSocketType(socket, SEND_CONNECT_RESPONSE);
        changeSocketType(client->second->getDestSocket(), CONNECT);
        break;
      case SocksRequestParser::DOMAIN_NAME:
        ares_gethostbyname(dns_channel_,
                           client->second->getParsedNameAddress().c_str(),
                           AF_INET, ares_callback,
                           new ares_arg(this, client->second->getDestSocket()));
        break;
      }
    }
    break;
  }
  case SEND_CONNECT_RESPONSE: {
    int status = client->second->sendConnectResponse();
    if (status < 0) {
#ifdef DEBUG
      std::cerr << "SEND_CONNECT_RESPONSE: Connection is rejected!"
                << std::endl;
#endif
      deleteClient(client->second->getDestSocket());
      deleteClient(socket);
      return -1;
    } else if (status > 0) {
#ifdef DEBUG
      std::cerr << "SEND_CONNECT_RESPONSE: Successfully send a response!"
                << std::endl;
#endif
      changeSocketType(socket, DATA_EXCHANGE_CLIENT);
    }
  }
  case CONNECT: {
    int error = client->second->connectDestHost();
    if (error == -1) {
#ifdef DEBUG
      std::cerr << "CONNECT: Connection is rejected!" << std::endl;
#endif
      deleteClient(client->second->getSocket());
      deleteClient(socket);
      return -1;
    } else if (error > 0) {
#ifdef DEBUG
      std::cerr << "CONNECT: Connection is successful!" << std::endl;
#endif
      changeSocketType(socket, DATA_EXCHANGE_DEST);
    }
    break;
  }
  case DATA_EXCHANGE_CLIENT: {
    int error = client->second->exchangeClientData(writable);
    if (error < 0) {
      deleteClient(client->second->getDestSocket());
      deleteClient(socket);
      return 0;
    }
    break;
  }
  case DATA_EXCHANGE_DEST: {
    int error = client->second->exchangeDestData(writable);
    if (error < 0) {
      deleteClient(client->second->getSocket());
      deleteClient(socket);
      return 0;
    }
    break;
  }
  case NONE:
    break;
  }

  return 0;
}

bool Server::changeSocketType(const int &socket, const SocketType &new_type) {
  auto client = deleteClient(socket);
  if (client == nullptr) {
    return false;
  }
  clients_.insert(std::make_pair(Socket(socket, new_type), client));

  return true;
}

void Server::ares_callback(void *arg, int status, int timeouts,
                           struct hostent *hostent) {
  auto *info = reinterpret_cast<ares_arg *>(arg);
  auto clients = info->instance->clients_;
  auto client = clients.find(Socket(info->socket_))->second;
  info->instance->changeSocketType(client->getSocket(), SEND_CONNECT_RESPONSE);
  if (status == ARES_SUCCESS) {
#ifdef DEBUG
    std::cerr << "DNS: Name is resolved!" << std::endl;
#endif
    client->setDestAddress(*(in_addr *)(hostent->h_addr_list[0]),
                           client->getParsedPort());
    info->instance->changeSocketType(info->socket_, CONNECT);
  } else {
    client->setConnectResponseStatus(-1);
  }

  delete info;
}