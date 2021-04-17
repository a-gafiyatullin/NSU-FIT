#pragma once

#include <queue>
#include <mutex>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <exception>
#include <unistd.h>
#include <semaphore.h>

class HTTP_Server {
  private:
    static std::string _response;

    HTTP_Server(const int &threads_num);
    ~HTTP_Server();

    static HTTP_Server* _server;
    bool _stop;
    int _server_socket;

    std::queue<int> _connections_q;
    std::mutex _connections_m;
    sem_t _semaphore;

    std::vector<std::thread> _thread_pool;

    void _response_thread();
  public:
    static HTTP_Server* init(const int &threads_num);

    void accept_connections();
    void stop() { _stop = true; close(_server_socket); }

    static void release();
};
