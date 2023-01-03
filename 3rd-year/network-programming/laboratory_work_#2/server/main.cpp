#include "Server.h"
#include <csignal>
#include <iostream>
#include <unistd.h>

bool stop = false;

void do_stop(int) { stop = true; }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: server [port]" << std::endl;
    return -1;
  }
  in_port_t port = atoi(argv[1]);
  if (port < 1024) {
    std::cerr << "port must be >= 1024" << std::endl;
    return -1;
  }

  if (chdir("uploads") < 0) {
    if (mkdir("uploads", S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) < 0) {
      std::perror("mkdir");
      return -1;
    }
    if (chdir("uploads") < 0) {
      std::perror("chdir");
      return -1;
    }
  }

  struct sigaction act {};
  act.sa_handler = do_stop;
  sigemptyset(&act.sa_mask);
  act.sa_flags = ~SA_RESTART;
  if (sigaction(SIGINT, &act, nullptr) < 0) {
    std::perror("sigaction");
    return -1;
  }

  Server server(port);
  if (server.bind() < 0) {
    std::perror("Server::bind");
    return -1;
  }

  while (!stop) {
    server.accept();
  }

  return 0;
}