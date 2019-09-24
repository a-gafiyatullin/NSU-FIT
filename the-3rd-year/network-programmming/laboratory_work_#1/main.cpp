#include <arpa/inet.h>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define IPv4 4
#define SEND_TIME_OUT 3
#define DEATH_MULT 3

int main(int argc, char *argv[]) {

  const char *error_msg = "usage: self-detect -t 4/6 -p PORT -a IPv4/IPv6 -i "
                          "interface_name(for IPv6)";

  if (argc < 4) {
    std::cerr << error_msg << std::endl;
    return -1;
  }

  int domain = -1; // AF_INET or AF_INET6
  in_port_t port = 0;
  char *address = nullptr;
  char *interface = nullptr;

  char **argv_t = argv + 1;
  while (*argv_t) {
    if ((*argv_t)[0] == '-') {
      switch ((*argv_t)[1]) {
      case 't':
        domain = (atoi(*++argv_t) == IPv4 ? AF_INET : AF_INET6);
        break;
      case 'p':
        port = atoi(*++argv_t);
        break;
      case 'a':
        address = *++argv_t;
        break;
      case 'i':
        interface = *++argv_t;
        break;
      default:
        std::cerr << error_msg << std::endl;
        return -1;
      }
    }
    argv_t++;
  }
  if (domain == -1 || port < 1024 || address == nullptr ||
      (domain == AF_INET6 && interface == nullptr)) {
    std::cerr << error_msg << std::endl;
    return -1;
  }

  // create local socket and set up local socket
  int socket_handler = socket(domain, SOCK_DGRAM, 0);
  if (socket_handler < 0) {
    std::perror("socket");
    return -1;
  }
  if (fcntl(socket_handler, F_SETFL, O_NONBLOCK) < 0) {
    std::perror("fcntl");
    return -1;
  }

  // set up an information about remote socket
  sockaddr *remote_addr;
  sockaddr *remote_host_info;
  void *address_cast_remote_host;
  socklen_t addr_len =
      (domain == AF_INET ? sizeof(sockaddr_in) : sizeof(sockaddr_in6));
  if ((remote_addr = (sockaddr *)calloc(1, addr_len)) == nullptr) {
    std::perror("malloc");
    return -1;
  }
  if ((remote_host_info = (sockaddr *)calloc(1, addr_len)) == nullptr) {
    std::perror("malloc");
    return -1;
  }
  if (domain == AF_INET) {
    ((sockaddr_in *)remote_addr)->sin_port = htons(port);
    ((sockaddr_in *)remote_addr)->sin_family = AF_INET;
    if (inet_pton(AF_INET, address, &((sockaddr_in *)remote_addr)->sin_addr) <
        0) {
      std::perror("inet_pton");
      return -1;
    }
    address_cast_remote_host = &((sockaddr_in *)remote_host_info)->sin_addr;
  } else {
    ((sockaddr_in6 *)remote_addr)->sin6_port = port;
    ((sockaddr_in6 *)remote_addr)->sin6_family = AF_INET6;
    ((sockaddr_in6 *)remote_addr)->sin6_flowinfo = 0;
    ((sockaddr_in6 *)remote_addr)->sin6_scope_id = if_nametoindex(interface);
    if (inet_pton(AF_INET6, address,
                  &((sockaddr_in6 *)remote_addr)->sin6_addr) < 0) {
      std::perror("inet_pton");
      return -1;
    }
    address_cast_remote_host = &((sockaddr_in6 *)remote_host_info)->sin6_addr;
  }

  // set up an information about local socket
  auto *local_addr = (sockaddr *)remote_addr;
  if (bind(socket_handler, local_addr, addr_len) < 0) {
    std::perror("bind");
    return -1;
  }

  // info about all nodes in the network
  std::map<std::string, time_t> nodes_info;

  time_t prev_send_time = time(nullptr);
  time_t current_time;
  timeval block_time;
  int ready_cnt;
  char *remote_host_address = (char *)malloc(INET6_ADDRSTRLEN);
  while (1) {
    current_time = time(nullptr);
    if (current_time - prev_send_time >= SEND_TIME_OUT) {
      sendto(socket_handler, nullptr, 0, 0, remote_addr, addr_len);
      prev_send_time = current_time;
    }

    // set up a socket blocking info
    block_time.tv_sec = SEND_TIME_OUT;
    block_time.tv_usec = 0;
    fd_set ready_to_read;
    FD_ZERO(&ready_to_read);
    FD_SET(socket_handler, &ready_to_read);
    if ((ready_cnt = select(socket_handler + 1, &ready_to_read, nullptr,
                            nullptr, &block_time)) < 0) {
      break;
    } else if (ready_cnt == 1) {
      bool is_updated = false;
      recvfrom(socket_handler, nullptr, 0, 0, remote_host_info, &addr_len);
      if (inet_ntop(remote_host_info->sa_family, address_cast_remote_host,
                    remote_host_address, INET6_ADDRSTRLEN) != nullptr) {
        std::string key(remote_host_address);
        auto iter = nodes_info.find(key);
        if (iter == nodes_info.end()) {
          is_updated = true;
          nodes_info.insert(std::make_pair(key, time(nullptr)));
        } else {
          iter->second = time(nullptr);
        }
      }
      std::vector<std::string> to_delete;
      for (const auto &elem : nodes_info) {
        if (time(nullptr) - elem.second >= DEATH_MULT * SEND_TIME_OUT) {
          is_updated = true;
          to_delete.push_back(elem.first);
        }
      }
      for (const auto &del : to_delete) {
        nodes_info.erase(del);
      }
      to_delete.clear();
      if (is_updated) {
        for (const auto &elem : nodes_info) {
          std::cout << elem.first << std::endl;
        }
        std::cout << std::endl;
      }
    }
  }

  close(socket_handler);
  free(remote_addr);
  free(remote_host_address);
  free(remote_host_info);
  return 0;
}