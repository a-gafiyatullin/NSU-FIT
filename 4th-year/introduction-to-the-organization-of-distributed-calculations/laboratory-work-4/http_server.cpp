#include "http_server.h"

#ifdef DEBUG
#include <iostream>
#endif

std::string HTTP_Server::_response = "HTTP/1.1 200 OK\r\n"
"Content-Type: text/html; charset=UTF-8\r\n\r\n"
"<!DOCTYPE html><html><head><title>Bye-bye baby bye-bye</title>"
"<style>body { background-color: #111 }"
"h1 { font-size:4cm; text-align: center; color: black;"
" text-shadow: 0 0 2mm red}</style></head>"
"<body><h1>Hello, world!</h1></body></html>\r\n";

HTTP_Server* HTTP_Server::_server = nullptr;

HTTP_Server::HTTP_Server(const int &threads_num) :
  _stop(false) {

  // set up the socket for accept
  _server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(_server_socket < 0) {
    throw std::runtime_error("HTTP_Server::HTTP_Server: can't open a socket!");
  }

  int data = 1;
  if(setsockopt(_server_socket, SOL_SOCKET, SO_REUSEADDR, &data, sizeof(int)) < 0) {
    close(_server_socket);
    throw std::runtime_error("HTTP_Server::HTTP_Server: can't set socket properties!");
  }

  struct sockaddr_in svr_addr;
  int port = 8080;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);

  if (bind(_server_socket, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(_server_socket);
    throw std::runtime_error("HTTP_Server::HTTP_Server: can't bind the socket!");
  }

  if(listen(_server_socket, SOMAXCONN) < 0) {
    close(_server_socket);
    throw std::runtime_error("HTTP_Server::HTTP_Server: can't listen the socket!");
  }

  for(int i = 0; i < threads_num; i++) {
    _thread_pool.push_back(std::thread(&HTTP_Server::_response_thread, this));
  }

  sem_init(&_semaphore, 0, 0);
}

HTTP_Server::~HTTP_Server() {
  for(size_t i = 0; i < _thread_pool.size(); i++) {
    sem_post(&_semaphore);
  }

  for(size_t i = 0; i < _thread_pool.size(); i++) {
    _thread_pool[i].join();
#ifdef DEBUG
    std::cout << "Stop thread: " << i << std::endl;
#endif
  }

  sem_destroy(&_semaphore);
}

void HTTP_Server::accept_connections() {
  struct sockaddr_in cli_addr;
  socklen_t sin_len = sizeof(cli_addr);

  while (true) {
    int client_fd = accept(_server_socket, (struct sockaddr *) &cli_addr, &sin_len);

    if(_stop) {
      break;
    }
#ifdef DEBUG
    std::cout << "New connection: " << client_fd << std::endl;
#endif

    _connections_m.lock();
    _connections_q.push(client_fd);
    _connections_m.unlock();

    sem_post(&_semaphore);
  }
}

void HTTP_Server::_response_thread() {
  static const int BUFFSZIE = 8192;
  static const std::string GET_REQ = "GET";
  static const int GET_REQ_LEN = 3;
  static const int RESPONSE_LEN = _response.length();
  char buffer[BUFFSZIE];

  while(true) {
    sem_wait(&_semaphore);
    if(_stop) {
      break;
    }

    _connections_m.lock();
    int client_fd = _connections_q.front();
    _connections_q.pop();
    _connections_m.unlock();

#ifdef DEBUG
    std::cout << "Process connection: " << client_fd << " in thread " << std::this_thread::get_id() << std::endl;
#endif

    bool get_req = false;
    int total_len = 0, len = 0;
    while(total_len < GET_REQ_LEN) {
      len = read(client_fd, buffer + total_len, BUFFSZIE - total_len);
      if(len <= 0) {
        break;
      }
      total_len += len;
    }

#ifdef DEBUG
    std::cout << "Request from : " << client_fd
              << std::endl << std::string(buffer, total_len) << std::endl;
#endif

    if(std::string(buffer, 3) == GET_REQ) {
#ifdef DEBUG
    std::cout << "It's a GET request from: " << client_fd << std::endl;
#endif
      get_req = true;
    }

    if(get_req) {

#ifdef WORK
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif

      len = 0;
      total_len = 0;
      while(total_len < RESPONSE_LEN) {
        len = write(client_fd, _response.c_str() + total_len, RESPONSE_LEN - total_len);
        if(len <= 0) {
          break;
        }
        total_len += len;
      }
    }

#ifdef DEBUG
    std::cout << "Sent response to: " << client_fd << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
#endif

    close(client_fd);
  }
}

HTTP_Server *HTTP_Server::init(const int &threads_num) {
  return (_server == nullptr ? (_server = new HTTP_Server(threads_num)) : _server);
}

void HTTP_Server::release() {
  delete _server;
  _server = nullptr;
}
