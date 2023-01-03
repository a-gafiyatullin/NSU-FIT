#pragma once

#include "../utility/IPv4Address.h"
#include "Snake.h"
#include <chrono>
#include <mutex>
#include <queue>
#include <sys/socket.h>
#include <thread>

#define MULTICAST_PORT 9192
#define MULTICAST_ADDRESS "239.192.0.4"
#define ERROR_DELAY 3

using namespace std::chrono;

class Client;

class Server {
private:
  // execution info
  static Server *server_;
  std::thread announcement_thread_;
  std::thread run_thread_;
  bool stop_flag_;

  // connection info
  std::map<int32_t, IPv4Address> clients_addresses_;
  int server_socket_;
  IPv4Address server_address_;

  // state info
  std::map<int32_t, std::pair<std::queue<GameMessage>, int64_t>> msg_queues_;
  GameConfig config_;
  GameState current_state_;
  std::mutex player_mutex_;
  int64_t last_state_update_;
  int32_t max_player_id_;
  int64_t msg_seq_;
  bool no_deputy_flag_;

  // local game info
  Client *local_client_;
  GameField *field_;

  void announce();

  GameMessage makeChangeRoleMessage(const NodeRole &sender_role,
                                    const NodeRole &receiver_role,
                                    const int32_t &receiver_id);

  // return messages from the clients
  std::vector<std::pair<IPv4Address, GameMessage>> getGameMessages();

  // return messages to the clients
  std::vector<GameMessage>
  step(const std::vector<std::pair<IPv4Address, GameMessage>> &msgs);

  void sendGameMessages(const std::vector<GameMessage> &msgs);

  void genFood();

  void chooseNewDeputy();

  std::pair<int, std::string> createNewPlayer(const std::string &name,
                                              const IPv4Address &address,
                                              const PlayerType &type,
                                              const NodeRole &role);

  // this constructor create new game, new local client and new game field
  Server(const int32_t &width, const int32_t &height,
         const int32_t &food_static, const int32_t &food_per_player,
         const int32_t &state_delay_ms, const float &dead_food_prob,
         const int32_t &ping_delay_ms, const int32_t &node_timeout_ms,
         const std::string &name);

  // this constructor set local client role as MASTER
  Server(const GameState &state, const GameConfig &config, int socket);

public:
  static Server *getInstance(const GameState &state, const GameConfig &config,
                             int socket);

  static Server *
  getInstance(const int32_t &width, const int32_t &height,
              const int32_t &food_static, const int32_t &food_per_player,
              const int32_t &state_delay_ms, const float &dead_food_prob,
              const int32_t &ping_delay_ms, const int32_t &node_timeout_ms,
              const std::string &name);

  inline void stop(const bool &stop_flag) { stop_flag_ = stop_flag; }

  inline IPv4Address getServerAddress() const { return server_address_; }

  ~Server() = default;

  // utility methods
  inline static int64_t getTime() {
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch())
        .count();
  }

  void run();
};

class Client {
private:
  // execution info
  static Client *client_;
  std::thread sendMove_thread_;
  std::thread receiveGameState_thread_;
  bool stop_flag_;

  // client info and state
  int local_socket_;
  IPv4Address client_address_;
  int32_t client_id_;
  bool only_view_;
  NodeRole role_;
  std::string player_name_;
  // time of the last message from this client to the sever
  int64_t client_last_ping_;

  // server and deputy info and state
  static Server *local_server_;
  IPv4Address server_address_;
  IPv4Address deputy_address_;
  int32_t server_id_;
  int64_t msg_seq_;
  std::queue<GameMessage> messages_;
  std::mutex message_mutex_;
  // time of the last message from the sever to this client
  int64_t server_last_ping_;

  // game info and state
  GameField *field_;
  GameConfig config_;
  GameState state_;

  // interaction methods
  bool registration();
  std::pair<GameMessage, int32_t> readMove();
  void sendMove();
  void receiveGameState();
  void becomeMaster();

  // user interface methods
  void updateGameField(const GameState &state);
  static std::string roleToString(const NodeRole &role);

  Client(const IPv4Address &server_address, const std::string &name,
         const bool &only_view, const GameMessage &game_announce,
         const NodeRole &role);

public:
  static Client *getInstance(const IPv4Address &server_address,
                             const std::string &name, const bool &only_view,
                             const GameMessage &game_announce,
                             const NodeRole &role);

  inline static Client *getInstance() { return client_; }

  inline IPv4Address getClientAddress() const { return client_address_; }

  inline int32_t getId() const { return client_id_; }

  void run();
};
