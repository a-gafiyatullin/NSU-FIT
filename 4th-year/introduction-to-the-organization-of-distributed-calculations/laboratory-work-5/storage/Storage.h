#pragma once

#include "../utils/HashUtils.h"
#include <arpa/inet.h>
#include <map>
#include <thread>
#include <fstream>
#include "../utils/FileSystemUtils.h"

#ifdef DEBUG
#include <iostream>
#endif

class Storage {
  protected:
    struct info {
        uint32_t  _addr;
        in_port_t _port;
    };

    static Storage*                   _instance;

    std::map<value_hash, info>        _sources;
    std::map<value_hash, std::string> _resources;

    sockaddr_in                       _addr;
    int                               _server_socket;
    int                               _client_socket;

    std::thread                       _thread;
    volatile int                      _stop;

    void thread_loop();

    Storage(const in_port_t &port);
  public:
    static Storage* init(const in_port_t &port);
    static Storage* instance();

    void add_key_value(const value_hash &hash, const uint32_t &addr, const in_port_t &port);
    std::pair<bool, info> get_value(const value_hash &hash);

    void add_resource(const std::string &name, const std::string &path);
    bool download_resource(const std::string &name, const std::string &dst);

    static void release();
};
