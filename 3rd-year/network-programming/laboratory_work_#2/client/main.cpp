#include "Client.h"
#include <iostream>

int main(int argc, char *argv[]) {
  const char *error_msg =
      "usage: client (-i IPv4)/(-d host-name) -p PORT -f file-path";

  if (argc < 4) {
    std::cerr << error_msg << std::endl;
    return -1;
  }

  in_port_t port = 0;
  char *address = nullptr;
  char *file_path = nullptr;
  bool isIp = false;

  char **argv_t = argv + 1;
  while (*argv_t) {
    if ((*argv_t)[0] == '-') {
      switch ((*argv_t)[1]) {
      case 'i':
        isIp = true;
        address = *++argv_t;
        break;
      case 'p':
        port = atoi(*++argv_t);
        break;
      case 'd':
        address = *++argv_t;
        break;
      case 'f':
        file_path = *++argv_t;
        break;
      default:
        std::cerr << error_msg << std::endl;
        return -1;
      }
    }
    argv_t++;
  }

  Client client;
  if (client.bind() < 0) {
    std::perror("Client::bind");
    return -1;
  }
  if (client.connect(address, port, isIp) < 0) {
    std::perror("Client::connect");
    return -1;
  }
  if (client.upload(file_path) < 0) {
    std::perror("Client::upload");
    return -1;
  } else {
    std::cout << "Upload succeed!" << std::endl;
  }

  return 0;
}