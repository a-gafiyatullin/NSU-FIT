#include "IPv4Address.h"

IPv4Address::IPv4Address(const std::string &address, in_port_t port)
    : ip_address_string(INET_ADDRSTRLEN, '\0') {
  this->address.sin_family = AF_INET;
  setAddress(address, port);
}

bool IPv4Address::operator<(const IPv4Address &iPv4Address) const {
  return getCanonicalAddress() < iPv4Address.getCanonicalAddress();
}

std::string IPv4Address::getCanonicalAddress() const {
  return ip_address_string + ":" + std::to_string(getPort());
}

IPv4Address::IPv4Address(in_port_t port)
    : ip_address_string(INET_ADDRSTRLEN, '\0') {
  this->address.sin_family = AF_INET;
  this->address.sin_addr.s_addr = INADDR_ANY;
  this->address.sin_port = htons(port);
}

void IPv4Address::setAddress(const std::string &string, in_port_t port) {
  ip_address_string = string;
  if (inet_pton(AF_INET, string.c_str(), &this->address.sin_addr) < 0) {
    throw std::invalid_argument("can't cast IP string!");
  }
  this->address.sin_port = htons(port);
}

void IPv4Address::update() {
  if (inet_ntop(AF_INET, &address.sin_addr, (char *)ip_address_string.c_str(),
                INET_ADDRSTRLEN) == nullptr) {
    throw std::invalid_argument("can't cast IP address to string!");
  }
  ip_address_string =
      ip_address_string.substr(0, strlen(ip_address_string.c_str()));
}

bool IPv4Address::operator!=(const IPv4Address &iPv4Address) const {
  return !(*this == iPv4Address);
}

bool IPv4Address::operator==(const IPv4Address &iPv4Address) const {
  return getCanonicalAddress() == iPv4Address.getCanonicalAddress();
}

IPv4Address &IPv4Address::operator=(const IPv4Address &other) {
  ip_address_string = other.ip_address_string;
  memcpy(&address, &other.address, sizeof(sockaddr_in));
  return *this;
}

std::ostream &operator<<(std::ostream &s, const IPv4Address &address) {
  s << address.getCanonicalAddress();
  return s;
}
