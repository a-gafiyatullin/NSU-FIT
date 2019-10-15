#include "Server.h"

Server::Server(in_port_t port)
    : port(port), socket(::socket(AF_INET, SOCK_STREAM, 0)) {
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_port = port;
  serverAddress.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server() {
  for (auto t : clientThreads) {
    pthread_join(*t.first, nullptr);
    delete t.first;
    delete t.second;
  }

  close(socket);
}

int Server::bind() {
  if (socket < 0) {
    return -1;
  }

  if (::bind(socket, (sockaddr *)(&serverAddress), sizeof(serverAddress)) < 0) {
    return -1;
  }

  if (listen(socket, SOMAXCONN) < 0) {
    return -1;
  }

  return 0;
}

int Server::accept() {
  sockaddr_in client_addr{};
  socklen_t addr_len = sizeof(client_addr);
  int clientSocket = ::accept(socket, (sockaddr *)(&client_addr), &addr_len);
  if (clientSocket < 0) {
    return -1;
  }
  auto *client_info = new ClientProcessing(clientSocket);
  auto *client_thread = new pthread_t;
  if (pthread_create(client_thread, nullptr,
                     &ClientProcessing::clientProcessing,
                     (void *)client_info) < 0) {
    return -1;
  }

  clientThreads.emplace_back(client_thread, client_info);

  return 0;
}
