#include "IPv4Address.h"

bool IPv4Address::operator<(const IPv4Address &iPv4Address) const {
  return getFullAddress() < iPv4Address.getFullAddress();
}

void IPv4Address::updateStringAddress() {
  if (inet_ntop(AF_INET, &address_.sin_addr, (char *)ip_address_string_.c_str(),
                INET_ADDRSTRLEN + 1) == nullptr) {
    throw std::invalid_argument("can't cast IP address to string!");
  }
  ip_address_string_ =
      ip_address_string_.substr(0, strlen(ip_address_string_.c_str()));
}

bool IPv4Address::operator!=(const IPv4Address &iPv4Address) const {
  return !(*this == iPv4Address);
}

bool IPv4Address::operator==(const IPv4Address &iPv4Address) const {
  return getFullAddress() == iPv4Address.getFullAddress();
}

IPv4Address &IPv4Address::operator=(const IPv4Address &other) {
  ip_address_string_ = other.ip_address_string_;
  memcpy(&address_, &other.address_, sizeof(sockaddr_in));

  return *this;
}

std::ostream &operator<<(std::ostream &s, const IPv4Address &address) {
  s << address.getFullAddress();

  return s;
}

IPv4Address::IPv4Address(const sockaddr_in &address)
    : ip_address_string_(INET_ADDRSTRLEN + 1, '\0') {
  memcpy(&address_, &address, sizeof(sockaddr_in));
  updateStringAddress();
}

IPv4Address::IPv4Address(const std::string &address, const in_port_t &port)
    : ip_address_string_(address) {
  address_.sin_family = AF_INET;
  address_.sin_port = htons(port);
  address_.sin_addr.s_addr = inet_addr(ip_address_string_.c_str());
}

std::string IPv4Address::getFullAddress() const {
  return getAddress() + ":" + std::to_string(getPort());
}
