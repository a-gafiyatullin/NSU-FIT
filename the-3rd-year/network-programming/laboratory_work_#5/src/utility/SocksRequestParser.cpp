#include "SocksRequestParser.h"

SocksRequestParser::SocksRequestParser()
    : total_data_len_(0), request_min_size_(0), port_(0), address_type_(0) {}

int SocksRequestParser::parseAuthRequest(const size_t &delta_buff_len) {
  total_data_len_ += delta_buff_len;
  if (total_data_len_ > SOCKS_VER_BYTE) {
    if (data_[SOCKS_VER_BYTE] != SOCKS5) {
      return -2;
    }
  }
  if (total_data_len_ <= NMETHODS_BYTE) {
    return 0;
  } else {
    int nmethods = data_[NMETHODS_BYTE];
    if (total_data_len_ < nmethods + NMETHODS_BYTE + 1) {
      return 0;
    } else {
      total_data_len_ = 0;
      return nmethods + NMETHODS_BYTE + 1;
    }
  }
}

int SocksRequestParser::parseConnectRequest(const size_t &delta_buff_len) {
  total_data_len_ += delta_buff_len;
  if (total_data_len_ > SOCKS_VER_BYTE) {
    if (data_[SOCKS_VER_BYTE] != SOCKS5) {
      return -2;
    }
  }
  if (total_data_len_ > COMMAND_TYPE_BYTE) {
    if (data_[COMMAND_TYPE_BYTE] != CONNECT) {
      return -3;
    }
  }
  if (total_data_len_ <= ATYP_BYTE) {
    return 0;
  } else {
    request_min_size_ = ATYP_BYTE + 1;
    switch (data_[ATYP_BYTE]) {
    case IPv4_ATYP:
      request_min_size_ += IPv4_LENGTH;
      break;
    case IPv6_ATYP:
      return -4;
    case DOMAIN_NAME:
      if (total_data_len_ > request_min_size_) {
        request_min_size_ += data_[request_min_size_] + 1;
      } else {
        return 0;
      }
    }
    request_min_size_ += PORT_FIELD_LENGTH;
    if (total_data_len_ < request_min_size_) {
      return 0;
    } else {
      switch (data_[ATYP_BYTE]) {
      case IPv4_ATYP:
        ipv4_address_.s_addr =
            htonl(((unsigned char)data_[IP_START_BYTE] << THREE_OCTET_SHIFT) |
                  ((unsigned char)data_[IP_START_BYTE + 1] << TWO_OCTET_SHIFT) |
                  ((unsigned char)data_[IP_START_BYTE + 2] << ONE_OCTET_SHIFT) |
                  (unsigned char)data_[IP_START_BYTE + 3]);
        address_type_ = IPv4_ATYP;
        break;
      case DOMAIN_NAME:
        name_address_ =
            std::string(data_ + IP_START_BYTE + 1, data_[IP_START_BYTE]);
        address_type_ = DOMAIN_NAME;
        break;
      }
      port_ =
          htons(((unsigned char)data_[request_min_size_ - PORT_FIELD_LENGTH]
                 << ONE_OCTET_SHIFT) |
                (unsigned char)data_[request_min_size_ - PORT_FIELD_LENGTH + 1]);

      return request_min_size_;
    }
  }
}

std::pair<char *, int>
SocksRequestParser::getConnectAnswer(const sockaddr_in *address,
                                     int answer_type) {
  char *answer = new char[ATYP_BYTE + IPv4_LENGTH + PORT_FIELD_LENGTH + 1];
  answer[SOCKS_VER_BYTE] = SOCKS5;
  answer[COMMAND_TYPE_BYTE] = answer_type;
  answer[2] = 0;
  answer[ATYP_BYTE] = IPv4_ATYP;
  if (address != nullptr) {
    int address_bytes = ntohl(address->sin_addr.s_addr);
    answer[IP_START_BYTE] = (address_bytes & FIRST_OCTET) >> THREE_OCTET_SHIFT;
    answer[IP_START_BYTE + 1] =
        (address_bytes & SECOND_OCTET) >> TWO_OCTET_SHIFT;
    answer[IP_START_BYTE + 2] =
        (address_bytes & THIRD_OCTET) >> ONE_OCTET_SHIFT;
    answer[IP_START_BYTE + 3] = address_bytes & FOURTH_OCTET;
    answer[IP_START_BYTE + 4] =
        (ntohs(address->sin_port) & THIRD_OCTET) >> ONE_OCTET_SHIFT;
    answer[IP_START_BYTE + 5] = ntohs(address->sin_port) & FOURTH_OCTET;
  } else {
    memset(answer + IP_START_BYTE, 0, IPv4_LENGTH + PORT_FIELD_LENGTH + 1);
  }

  return std::make_pair(answer,
                        ATYP_BYTE + IPv4_LENGTH + PORT_FIELD_LENGTH + 1);
}
