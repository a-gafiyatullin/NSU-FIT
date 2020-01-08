#include "ClientProcessing.h"

ClientProcessing::ClientProcessing(const int &socket)
    : client_socket_(socket), dest_socket_(::socket(AF_INET, SOCK_STREAM, 0)),
      start_to_connect_(false), connection_error_(-1),
      connection_response_(nullptr), client_buffer_pos_(0),
      client_buffer_length_(0), dest_buffer_pos_(0), dest_buffer_length_(0),
      socks_header_(std::make_shared<SocksRequestParser>()) {
  if (dest_socket_ < 0) {
    throw std::domain_error("Client::Client : resource_socket < 0!");
  }
}

int ClientProcessing::readAuthHeader() {
  std::pair<char *, ssize_t> buffer = socks_header_->getDataBuffer();
  ssize_t length = recv(client_socket_, buffer.first, buffer.second, 0);
  if (length <= 0) {
    return -1;
  }

  int error = socks_header_->parseAuthRequest(length);

  if (error > 0) {
#ifdef DEBUG
    std::cerr << "Successfully read an auth header!" << std::endl;
#endif
    client_buffer_length_ = SocksRequestParser::getAuthAnswer().second;
    client_buffer_pos_ = 0;
  }

  return error < 0 ? -1 : error;
}

int ClientProcessing::sendAuthResponse() {
  auto answer = SocksRequestParser::getAuthAnswer();

  ssize_t req_len = client_buffer_length_ - client_buffer_pos_;
  ssize_t len =
      send(client_socket_, answer.first + client_buffer_pos_, req_len, 0);
  if (len <= 0) {
    return -1;
  }
  client_buffer_pos_ += len;
  if (client_buffer_length_ == client_buffer_pos_) {
    client_buffer_length_ = 0;
    client_buffer_pos_ = 0;
#ifdef DEBUG
    std::cerr << "Successfully send an auth header!" << std::endl;
#endif
    return len;
  }

  return 0;
}

int ClientProcessing::readConnectHeader() {
  std::pair<char *, ssize_t> buffer = socks_header_->getDataBuffer();
  ssize_t length = recv(client_socket_, buffer.first, buffer.second, 0);
  if (length <= 0) {
    return -1;
  }

  connection_error_ = socks_header_->parseConnectRequest(length);

  if (connection_error_ > 0) {
#ifdef DEBUG
    std::cerr << "Successfully read a connect header!" << std::endl;
#endif
  }

  return connection_error_;
}

int ClientProcessing::sendConnectResponse() {
  if (connection_response_ == nullptr) {
    int answer_type = SocksRequestParser::REQ_GRANTED;
    switch (connection_error_) {
    case -2:
      answer_type = SocksRequestParser::REQ_FAULT;
      break;
    case -3:
      answer_type = SocksRequestParser::COMMAND_NOT_FOUND;
      break;
    case -4:
      answer_type = SocksRequestParser::UNSUPPORTED_ADDRESS;
      break;
    }
    sockaddr_in client_sockaddr{};
    socklen_t client_sockaddr_len = sizeof(sockaddr_in);
    if (getsockname(client_socket_, (sockaddr *)&client_sockaddr,
                    &client_sockaddr_len) < 0) {
      return -1;
    }
    auto answer = SocksRequestParser::getConnectAnswer(
        (answer_type < 0 ? nullptr : &client_sockaddr), answer_type);
    connection_response_ = answer.first;
    client_buffer_length_ = answer.second;
    client_buffer_pos_ = 0;
  }

  ssize_t len = send(client_socket_, connection_response_ + client_buffer_pos_,
                     client_buffer_length_ - client_buffer_pos_, 0);
  if (len <= 0) {
    return -1;
  }
  client_buffer_pos_ += len;
  if (client_buffer_pos_ == client_buffer_length_) {
    delete[] connection_response_;
    connection_response_ = nullptr;
    client_buffer_length_ = 0;
    client_buffer_pos_ = 0;

#ifdef DEBUG
    std::cerr << "Successfully send a connect header!" << std::endl;
#endif

    if (connection_error_ < 0) {
      return -2;
    }

    return len;
  }

  return 0;
}

int ClientProcessing::connectDestHost() {
  if (!start_to_connect_) { // try to connect
    // non-blocking connect
    if (set_flag(dest_socket_, O_NONBLOCK) < 0) {
      return -1;
    }
    int error =
        connect(dest_socket_, (sockaddr *)&dest_address_, sizeof(sockaddr_in));
    start_to_connect_ = true;

    if (error < 0) {
      return (errno == EINPROGRESS ? -2 : -1);
    }
  }

  // check connection
  int error = 0;
  socklen_t error_len = sizeof(error);
  if (getsockopt(dest_socket_, SOL_SOCKET, SO_ERROR, &error, &error_len) < 0) {
    return -1;
  }
  if (error == 0) {
    if (reset_flag(dest_socket_, O_NONBLOCK) < 0) {
      return -1;
    }
#ifdef DEBUG
    std::cerr << "Successfully connected!" << std::endl;
#endif
    return 1;
  } else {
    return (errno == EINPROGRESS ? -2 : -1);
  }
}

int ClientProcessing::exchangeClientData(const bool &write) {
  if (!write) {
    char buffer[BUFSIZ];
    ssize_t length = recv(client_socket_, buffer, BUFSIZ, 0);
    if (length <= 0) {
      return -1;
    }

    char *array = new char[length];
    memcpy(array, buffer, length);
    dest_data_.push(std::make_pair(array, length));

    return length;
  } else {
    if (client_buffer_length_ == 0) {
      if (!client_data_.empty()) {
        client_buffer_length_ = client_data_.front().second;
      } else {
        return 0;
      }
    }
    auto data = client_data_.front();
    ssize_t len = send(client_socket_, data.first + client_buffer_pos_,
                       client_buffer_length_ - client_buffer_pos_, 0);
    if (len <= 0) {
      return -1;
    }
    client_buffer_pos_ += len;
    if (client_buffer_pos_ == client_buffer_length_) {
      delete[] client_data_.front().first;
      client_data_.pop();
      client_buffer_pos_ = 0;
      if (!client_data_.empty()) {
        client_buffer_length_ = client_data_.front().second;
      } else {
        client_buffer_length_ = 0;
      }
    }

    return len;
  }
}

int ClientProcessing::exchangeDestData(const bool &write) {
  if (!write) {
    char buffer[BUFSIZ];

    ssize_t length = recv(dest_socket_, buffer, BUFSIZ, 0);
    if (length <= 0) {
      return -1;
    }

    char *array = new char[length];
    memcpy(array, buffer, length);
    client_data_.push(std::make_pair(array, length));

    return length;
  } else {
    if (dest_buffer_length_ == 0) {
      if (!dest_data_.empty()) {
        dest_buffer_length_ = dest_data_.front().second;
      } else {
        return 0;
      }
    }
    auto data = dest_data_.front();

    ssize_t len = send(dest_socket_, data.first + dest_buffer_pos_,
                       dest_buffer_length_ - dest_buffer_pos_, 0);
    if (len <= 0) {
      return -1;
    }
    dest_buffer_pos_ += len;
    if (dest_buffer_pos_ == dest_buffer_length_) {
      delete[] dest_data_.front().first;
      dest_data_.pop();
      dest_buffer_pos_ = 0;
      if (!dest_data_.empty()) {
        dest_buffer_length_ = dest_data_.front().second;
      } else {
        dest_buffer_length_ = 0;
      }
    }

    return len;
  }
}

ClientProcessing::~ClientProcessing() {
  while (!client_data_.empty()) {
    delete[] client_data_.front().first;
    client_data_.pop();
  }
  while (!dest_data_.empty()) {
    delete[] dest_data_.front().first;
    dest_data_.pop();
  }
  close(dest_socket_);
  close(client_socket_);
}

void ClientProcessing::setDestAddress(const in_addr &address,
                                      const in_port_t &port) {
  dest_address_.sin_family = AF_INET;
  dest_address_.sin_port = port;
  dest_address_.sin_addr.s_addr = address.s_addr;
}
