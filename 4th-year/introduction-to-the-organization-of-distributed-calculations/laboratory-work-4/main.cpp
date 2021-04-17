#include "http_server.h"
#include <iostream>
#include<signal.h>

HTTP_Server *server = nullptr;

void stop_server(int sig) {
  if(server != nullptr) {
    server->stop();
  }
}

int main(int argc, char *argv[]) {
  if(argc < 2) {
    std::cerr << "usage: http-server [threads_num]" << std::endl;
    return -1;
  }

  signal(SIGINT, stop_server);

  server = HTTP_Server::init(atoi(argv[1]));
  server->accept_connections();

  server->release();
}
