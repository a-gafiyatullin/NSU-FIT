#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <string>

class IPv4Address {
private:
  std::string ip_address_string_;
  sockaddr_in address_{};

  void updateStringAddress();

public:
  explicit IPv4Address(const sockaddr_in &address);

  explicit IPv4Address() : ip_address_string_(INET_ADDRSTRLEN + 1, '\0') {}

  // address in format xxx.xxx.xxx.xxx:ppppp
  explicit IPv4Address(const std::string &address, const in_port_t &port);

  IPv4Address &operator=(const IPv4Address &other);

  [[nodiscard]] inline sockaddr_in *getSockaddrStruct() { return &address_; }

  [[nodiscard]] inline std::string getAddress() const {
    return ip_address_string_;
  }

  [[nodiscard]] inline in_port_t getPort() const {
    return ntohs(address_.sin_port);
  }

  [[nodiscard]] std::string getFullAddress() const;

  bool operator<(const IPv4Address &iPv4Address) const;

  bool operator==(const IPv4Address &iPv4Address) const;

  bool operator!=(const IPv4Address &iPv4Address) const;

  friend std::ostream &operator<<(std::ostream &s, const IPv4Address &address);
};