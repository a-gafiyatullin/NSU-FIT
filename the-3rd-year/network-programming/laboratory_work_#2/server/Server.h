#pragma once

#include "ClientProcessing.h"
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <vector>

class Server {
private:
  in_port_t port;
  int socket;
  sockaddr_in serverAddress;
  std::vector<std::pair<pthread_t *, ClientProcessing *>> clientThreads;

public:
  Server(in_port_t port);

  int bind();

  int accept();

  ~Server();
};