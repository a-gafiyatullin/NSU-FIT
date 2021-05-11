#include "Storage.h"
#include "../dht/NodeTable.h"

Storage* Storage::_instance = nullptr;

void Storage::thread_loop() {
  while (!_stop) {
    struct sockaddr_in cli_addr;
    socklen_t sin_len = sizeof(cli_addr);

    int client_fd = accept(_server_socket, (struct sockaddr *) &cli_addr, &sin_len);
    if(client_fd < 0) {
      continue;
    }

    value_hash hash(SHA_DIGEST_LENGTH);
    int len = read(client_fd, hash.data(), SHA_DIGEST_LENGTH);
    if(len != SHA_DIGEST_LENGTH) {
      close(client_fd);
      continue;
    }

    std::ifstream src(_resources[hash]);
    if(!src.is_open()) {
      close(client_fd);
      continue;
    }

    char buffer[BUFSIZ];
    while(!src.eof()) {
      src.read(buffer, BUFSIZ);
      std::streamsize bytes = src.gcount();

      write(client_fd, buffer, bytes);
    }

    close(client_fd);
  }
}

Storage::Storage(const in_port_t &port) {
  // set up the socket for accept
  _server_socket = socket(AF_INET, SOCK_STREAM, 0);
  _client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if(_server_socket < 0 || _client_socket < 0) {
    throw std::runtime_error("Storage::Storage: can't open a socket!");
  }

  struct timeval read_timeout;
  read_timeout.tv_sec = 0;
  read_timeout.tv_usec = 10;
  if(setsockopt(_server_socket, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout) < 0) {
    throw std::runtime_error("Storage::Storage: can't set the socket to non-blocking mode!");
  }

  struct sockaddr_in svr_addr;
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(port);

  if (bind(_server_socket, (struct sockaddr *) &svr_addr, sizeof(svr_addr)) == -1) {
    close(_server_socket);
    throw std::runtime_error("Storage::Storage: can't bind the socket!");
  }

  if(listen(_server_socket, SOMAXCONN) < 0) {
    close(_server_socket);
    throw std::runtime_error("Storage::Storage: can't listen the socket!");
  }

  _thread = std::thread(&Storage::thread_loop, this);
}

Storage *Storage::init(const in_port_t &port) {
  return (_instance == nullptr ? (_instance = new Storage(port)) : _instance);
}

Storage *Storage::instance() {
  return _instance;
}

void Storage::add_key_value(const value_hash &hash, const uint32_t &addr, const in_port_t &port) {
  _sources.insert(std::make_pair(hash, info{addr, port}));
}

std::pair<bool, Storage::info> Storage::get_value(const value_hash &hash) {
  auto val = _sources.find(hash);

  if(val == _sources.end()) {
    return std::make_pair(false, info{0, 0});
  }

  return std::make_pair(true, val->second);
}

void Storage::add_resource(const std::string &name, const std::string &path) {
  value_hash hash = calc_hash(name);

  _resources.insert(std::make_pair(hash, path));
  _sources.insert(std::make_pair(hash, info{0, _addr.sin_port}));
}

bool Storage::download_resource(const std::string &name, const std::string &dst) {
  value_hash hash = calc_hash(name);

  auto info = get_value(hash);
  if(info.first) {
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = info.second._addr;
    addr.sin_port = info.second._port;

    if(connect(_client_socket, (sockaddr *)&addr, sizeof (addr)) != 0) {
      return false;
    }

    std::string path = dst + "/" + name;
    std::ofstream dst_file(path);
    if(!dst_file.is_open()) {
      return false;
    }

    char buffer[BUFSIZ];
    int len = 0;

    if(write(_client_socket, hash.data(), SHA_DIGEST_LENGTH) != SHA_DIGEST_LENGTH) {
      return false;
    }

    while((len = read(_client_socket, buffer, BUFSIZ)) > 0) {
      dst_file.write(buffer, len);
    }

    dst_file.close();
    _resources.insert(std::make_pair(hash, path));

    return true;
  } else {
    bool found = NodeTable::instance()->value_lookup(hash);
    if(found) {
      return download_resource(name, dst);
    } else {
      return false;
    }
  }
}

void Storage::release() {
  _instance->_stop = true;
  _instance->_thread.join();
  delete _instance;
  _instance = nullptr;
}
