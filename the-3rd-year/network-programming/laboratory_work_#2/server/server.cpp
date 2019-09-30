#include "client_processing_thread.h"
#include <csignal>
#include <iostream>
#include <netinet/in.h>
#include <pthread.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

bool stop = false;

void do_stop(int) { stop = true; }

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "usage: server [port]\n" << std::endl;
    return -1;
  }
  int port = atoi(argv[1]);
  if (port < 0 || port > 1023) {
    std::cerr << "port must be >= 1024\n" << std::endl;
    return -1;
  }

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    std::perror("socket");
    return -1;
  }

  sockaddr_in server_addr{};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = INADDR_ANY;

  if (bind(server_socket, (sockaddr *)(&server_addr), sizeof(server_addr)) <
      0) {
    std::perror("bind");
    return -1;
  }

  if (listen(server_socket, SOMAXCONN) < 0) {
    std::perror("listen");
    return -1;
  }

  std::vector<pthread_t *> client_threads;

  if (signal(SIGINT, do_stop) == SIG_ERR) {
    std::perror("signal");
    return -1;
  }
  while (!stop) {
    sockaddr_in client_addr{};
    socklen_t addr_len = sizeof(client_addr);
    size_t client_socket =
        accept(server_socket, (sockaddr *)(&client_addr), &addr_len);
    if (client_socket < 0) {
      continue;
    }
    auto *new_client = new pthread_t;
    if (pthread_create(new_client, nullptr, client_processing,
                       (void *)client_socket) < 0) {
      close(client_socket);
      continue;
    }
    client_threads.push_back(new_client);
  }

  for (pthread_t *t : client_threads) {
    int *client_socket;
    pthread_join(*t, (void **)&client_socket);
    close(*client_socket);
    delete client_socket;
    delete t;
  }

  close(server_socket);
  return 0;
}