#pragma once

#include <arpa/inet.h>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <optional>
#include <string>

#define FIRST_OCTET 0xff000000
#define SECOND_OCTET 0x00ff0000
#define THIRD_OCTET 0x0000ff00
#define FOURTH_OCTET 0x000000ff

#define ONE_OCTET_SHIFT 8
#define TWO_OCTET_SHIFT 16
#define THREE_OCTET_SHIFT 24

class SocksRequestParser {
private:
  int total_data_len_;
  int request_min_size_;

  std::string name_address_;
  in_addr ipv4_address_;
  int address_type_;
  in_port_t port_;

  char data_[BUFSIZ]{};

  static const int SOCKS_VER_BYTE = 0;
  static const int SOCKS5 = 0x5;
  static const int NMETHODS_BYTE = 1;
  static const int NO_AUTH = 0x00;
  static const int CONNECT = 0x1;
  static const int IPv4_LENGTH = 4;
  static const int PORT_FIELD_LENGTH = 2;
  static const int COMMAND_TYPE_BYTE = 1;
  static const int IP_START_BYTE = 4;
  static const int ATYP_BYTE = 3;

  constexpr static const char auth_answer_[] = {SOCKS5, NO_AUTH};
  constexpr static const std::pair auth_answer_with_len_ =
      std::make_pair(const_cast<char *>(auth_answer_), 2);

public:
  static const int IPv4_ATYP = 0x01;
  static const int IPv6_ATYP = 0x04;
  static const int DOMAIN_NAME = 0x03;
  static const int REQ_GRANTED = 0x0;
  static const int REQ_FAULT = 0x1;
  static const int COMMAND_NOT_FOUND = 0x07;
  static const int UNSUPPORTED_ADDRESS = 0x08;

  SocksRequestParser();

  [[nodiscard]] inline std::string dataToString() const {
    return std::string(data_, total_data_len_);
  }

  [[nodiscard]] inline std::pair<char *, size_t> getDataBuffer() const {
    return std::make_pair((char *)(data_ + total_data_len_),
                          BUFSIZ - total_data_len_);
  }

  [[nodiscard]] inline std::pair<char *, size_t>
  getDataBuffer(size_t start) const {
    return std::make_pair((char *)(data_ + start), total_data_len_ - start);
  }

  int parseAuthRequest(const size_t &delta_buff_len);

  int parseConnectRequest(const size_t &delta_buff_len);

  [[nodiscard]] inline std::optional<std::string> getNameAddress() const {
    if (address_type_ == DOMAIN_NAME) {
      return name_address_;
    } else {
      return {};
    }
  }

  [[nodiscard]] inline std::optional<in_addr> getIPv4Address() const {
    if (address_type_ == IPv4_ATYP) {
      return ipv4_address_;
    } else {
      return {};
    }
  }

  [[nodiscard]] int getAddressType() const { return address_type_; }

  [[nodiscard]] inline in_port_t getPort() const { return port_; }

  inline static std::pair<char *, int> getAuthAnswer() {
    return auth_answer_with_len_;
  }

  static std::pair<char *, int> getConnectAnswer(const sockaddr_in *address,
                                                 int answer_type);

  ~SocksRequestParser() = default;
};