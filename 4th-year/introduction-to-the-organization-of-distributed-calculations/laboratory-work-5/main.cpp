#include "storage/Storage.h"
#include "dht/NodeTable.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cerr << "usage: dfs [dht-port] [io-port] [bootstrap node info file]" << std::endl;
    return 0;
  }

  std::string bootstrap_addr;
  in_port_t bootstrap_port;
  node_id bootstrap_node;

  if(argc == 4) {
    std::ifstream boostrap(argv[3]);
    if(!boostrap.is_open()) {
      std::cerr << "Can't load the bootstrap node info!" << std::endl;
      return -1;
    }

    bootstrap_node.resize(SHA_DIGEST_LENGTH);
    for(int i = 0; i < SHA_DIGEST_LENGTH; i++) {
      unsigned int num;
      boostrap >> num;
      bootstrap_node[i] = num;
    }
    boostrap >> bootstrap_addr;
    boostrap >> bootstrap_port;
  }

  NodeTable*  node_table = NodeTable::init(atoi(argv[1]), bootstrap_node, bootstrap_addr, bootstrap_port);
  std::cerr << "NodeTable started!" << std::endl;
  Storage*    storage    = Storage::init(atoi(argv[2]));
  std::cerr << "Storage started!" << std::endl;

  bool stop = false;
  while (!stop) {
    char command;
    std::cin >> command;

    switch (command) {
    case 'A':
    case 'a': {
      std::string file_name;
      std::string path;

      std::cin >> file_name;
      std::cin >> path;

      storage->add_resource(file_name, path);
      break;
    }

    case 'S':
    case 's':
      stop = true;
      break;

    case 'F':
    case 'f': {
      std::string file_name;
      std::string path;

      std::cin >> file_name;
      std::cin >> path;

      storage->download_resource(file_name, path);
      break;
    }

    }
  }

  node_table->release();
  std::cerr << "NodeTable stopped!" << std::endl;
  storage->release();
  std::cerr << "Storage stopped!" << std::endl;
}
