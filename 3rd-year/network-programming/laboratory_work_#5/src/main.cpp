#include "server/Server.h"
#include <csignal>
#include <getopt.h>
#include <iostream>

bool stop = false;

void sig_stop(int) { stop = true; }

int main(int argc, char *argv[]) {
  std::string error_msg = "usage: socks-proxy [-p port-number]";
  if (argc < 2) {
    std::cerr << error_msg << std::endl;
    return -1;
  }

  in_port_t port = 0;
  char c;
  while ((c = getopt(argc, argv, "p:")) != EOF) { // get server port number
    switch (c) {
    case 'p':
      port = atoi(optarg);
      break;
    default:
      std::cerr << error_msg << std::endl;
      return -1;
    }
  }

  // use SIGINT to finish program in the good way
  struct sigaction int_act;
  int_act.sa_handler = sig_stop;
  sigemptyset(&int_act.sa_mask);
  int_act.sa_flags = ~SA_RESTART;
  sigaction(SIGINT, &int_act, nullptr);
  // ignore SIGPIPE from sockets
  struct sigaction pipe_act;
  pipe_act.sa_handler = SIG_IGN;
  sigemptyset(&pipe_act.sa_mask);
  sigaction(SIGPIPE, &pipe_act, nullptr);

  Server *server = Server::getInstance(port);

  std::cout << "socks-proxy started at port " << port << std::endl;
  int server_socket = server->getServerSocket();
  while (!stop) {
    fd_set r, w, e;
    int max_socket =
        server->getSocketsTasks(w, r, e); // get commands for sockets
    if (select(max_socket + 1, &r, &w, &e, nullptr) <= 0) {
      continue;
    }
    ares_process(server->getDnsChannel(), &r, &w);
    if (FD_ISSET(server_socket, &r)) {
      server->execClientAction(server_socket, true);
    }

    std::map<Socket, std::shared_ptr<ClientProcessing>, compare> sockets =
        server->getClients();
    for (auto iter = sockets.begin(); iter != sockets.end(); iter++) {
      if (FD_ISSET(iter->first.socket_, &r)) {
        server->execClientAction(iter->first.socket_, false);
      }
      if (FD_ISSET(iter->first.socket_, &w)) {
        server->execClientAction(iter->first.socket_, true);
      }
    }
  }

  std::cout << "Finishing server..." << std::endl;
  delete server; // free server structures
  std::cout << "Server is finished" << std::endl;

  return 0;
}