#include "Node.h"
#include <iostream>

int main(int argc, char *argv[]) {
  const char *error_msg = "usage: tree-node -n [node-name] -l [loss-rate] -pl "
                          "[local-port] (-a [parent-ipv4] -pp [parent-port])";

  if (argc < 4) {
    std::cerr << error_msg << std::endl;
    return -1;
  }

  in_port_t local_port = 0;
  in_port_t parent_port = 0;
  int loss_rate = 0;
  char *node_name = nullptr;
  char *parent_address = nullptr;

  char **argv_t = argv + 1;
  while (*argv_t) {
    if ((*argv_t)[0] == '-') {
      switch ((*argv_t)[1]) {
      case 'p':
        if ((*argv_t)[2] == 'l') {
          local_port = atoi(*++argv_t);
        } else if ((*argv_t)[2] == 'p') {
          parent_port = atoi(*++argv_t);
        }
        break;
      case 'a':
        parent_address = *++argv_t;
        break;
      case 'n':
        node_name = *++argv_t;
        break;
      case 'l':
        loss_rate = atoi(*++argv_t);
        break;
      default:
        std::cerr << error_msg << std::endl;
        return -1;
      }
    }
    argv_t++;
  }
  if (local_port == 0 || node_name == nullptr) {
    std::cerr << error_msg << std::endl;
    return -1;
  }
  Node *node;
  if (parent_address != nullptr && parent_port != 0) {
    node = new Node(std::string(node_name), loss_rate, local_port,
                    std::string(parent_address), parent_port);
  } else {
    node = new Node(std::string(node_name), loss_rate, local_port);
  }

  node->exchangeMessages();

  delete node;
  return 0;
}