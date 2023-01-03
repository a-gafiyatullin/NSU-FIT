#pragma once

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <string>

class IPv4Address {
private:
  std::string ip_address_string;
  sockaddr_in address{};

public:
  IPv4Address(const std::string &address, in_port_t port);

  explicit IPv4Address(in_port_t port);

  IPv4Address() : ip_address_string(INET_ADDRSTRLEN, '\0') {
    address.sin_family = AF_INET;
  }

  IPv4Address &operator=(const IPv4Address &other);

  void setAddress(const std::string &string, in_port_t port);

  void update();

  [[nodiscard]] inline sockaddr *getSockaddrStruct() const {
    return (sockaddr *)&address;
  }

  [[nodiscard]] inline std::string getAddress() const {
    return ip_address_string;
  }

  [[nodiscard]] inline in_port_t getPort() const {
    return ntohs(address.sin_port);
  }

  bool operator<(const IPv4Address &iPv4Address) const;

  bool operator==(const IPv4Address &iPv4Address) const;

  bool operator!=(const IPv4Address &iPv4Address) const;

  [[nodiscard]] std::string getCanonicalAddress() const;

  friend std::ostream &operator<<(std::ostream &s, const IPv4Address &address);
};