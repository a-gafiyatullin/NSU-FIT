#pragma once

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

class Client {
private:
  int socket;
  sockaddr_in client{};
  sockaddr_in server{};

public:
  explicit Client();

  int bind();

  int connect(const std::string &address, int remotePort, bool isIp);

  int upload(const std::string &path);

  ~Client();
};