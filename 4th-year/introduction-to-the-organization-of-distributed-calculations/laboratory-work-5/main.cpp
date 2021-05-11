#include "storage/Storage.h"
#include "dht/NodeTable.h"
#include <iostream>
#include <signal.h>

bool stop = false;

void stop_server(int sig) {
  stop = true;
}

int main(int argc, char *argv[]) {
  if(argc < 3) {
    std::cerr << "usage: dfs [dht-port] [io-port]" << std::endl;
    return 0;
  }

  NodeTable*  node_table = NodeTable::init(atoi(argv[1]));
  std::cerr << "NodeTable started!" << std::endl;
  Storage*    storage    = Storage::init(atoi(argv[2]));
  std::cerr << "Storage started!" << std::endl;

  signal(SIGINT, stop_server);

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
    }
  }

  node_table->release();
  std::cerr << "NodeTable stopped!" << std::endl;
  storage->release();
  std::cerr << "Storage stopped!" << std::endl;
}
