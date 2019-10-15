#include "Client.h"
#include <iostream>
Client::Client() : socket(::socket(AF_INET, SOCK_STREAM, 0)) {
  client.sin_family = AF_INET;
  client.sin_port = 0;
  client.sin_addr.s_addr = INADDR_ANY;
}

int Client::bind() {
  if (socket < 0) {
    return -1;
  }

  if (::bind(socket, (sockaddr *)&client, sizeof(client)) < 0) {
    return -1;
  }

  return 0;
}

int Client::connect(const std::string &address, int port, bool isIp) {
  if (isIp) {
    server.sin_family = AF_INET;
    server.sin_port = port;
    if (inet_pton(AF_INET, address.c_str(), &server.sin_addr) < 0) {
      return -1;
    }
  } else {
    addrinfo *info;
    addrinfo request{};
    request.ai_flags = AI_ADDRCONFIG;
    request.ai_family = AF_INET;
    request.ai_protocol = SOCK_STREAM;
    request.ai_socktype = 0;
    request.ai_addr = nullptr;
    request.ai_addrlen = 0;
    request.ai_canonname = nullptr;
    request.ai_next = nullptr;
    if (getaddrinfo(address.c_str(), nullptr, &request, &info) < 0) {
      return -1;
    }
    server = *(sockaddr_in *)info->ai_addr;
    server.sin_port = port;
    freeaddrinfo(info);
  }

  if (::connect(socket, (sockaddr *)&server, sizeof(server)) < 0) {
    return -1;
  }

  return 0;
}

int Client::upload(const std::string &path) {
  int file = open(path.c_str(), O_RDONLY);
  if (file < 0) {
    return -1;
  }

  size_t last_slash = path.find_last_of('/');
  auto filename = path.substr(last_slash + 1);
  unsigned short namelen = filename.length();
  if (send(socket, &namelen, sizeof(short), 0) <= 0) {
    close(file);
    return -1;
  }
  if (send(socket, filename.c_str(), namelen, 0) <= 0) {
    close(file);
    return -1;
  }
  struct stat fileinfo;
  if (fstat(file, &fileinfo) < 0) {
    close(file);
    return -1;
  }
  if (send(socket, &fileinfo.st_size, sizeof(unsigned long long int), 0) < 0) {
    close(file);
    return -1;
  }

  char buffer[BUFSIZ];
  size_t len = 0;
  int error = 0;
  while ((len = read(file, buffer, BUFSIZ)) > 0) {
    if ((error = send(socket, buffer, len, 0)) < 0) {
      break;
    }
  }

  if (error < 0) {
    return -1;
  }
  if (recv(socket, buffer, sizeof("OK"), MSG_WAITALL) <= 0) {
    return -1;
  }
  if (std::string(buffer) != "OK") {
    errno = EPROTO;
    return -1;
  }

  close(file);
  return 0;
}

Client::~Client() { close(socket); }
