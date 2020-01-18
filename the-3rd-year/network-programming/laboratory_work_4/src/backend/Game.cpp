#include "Game.h"

Server *Server::server_ = nullptr;

void Server::genFood() {
  srand(time(nullptr));

  auto alive_snakes_amount = std::count_if(
      current_state_.snakes().begin(), current_state_.snakes().end(),
      [](const GameState_Snake &s) {
        return s.state() == GameState_Snake::ALIVE;
      });
  auto food_amount =
      config_.food_static() + (config_.food_per_player() * alive_snakes_amount);
  auto need_food_amount = food_amount - current_state_.foods().size();
  auto free_coord_amount = field_->getFreeCoordAmount();

  ssize_t i = 0;
  while (i < need_food_amount && free_coord_amount > 0) {
    GameState_Coord new_food;
    new_food.set_x(rand() % config_.width());
    new_food.set_y(rand() % config_.height());
    if (field_->isFreeCoord(new_food)) {
      current_state_.mutable_foods()->Add()->CopyFrom(new_food);
      i++;
      free_coord_amount--;
    }
  }
}

void Server::announce() {
  sockaddr_in multicast{};
  multicast.sin_port = htons(MULTICAST_PORT);
  multicast.sin_family = AF_INET;
  if (inet_pton(AF_INET, std::string(MULTICAST_ADDRESS).c_str(),
                &multicast.sin_addr) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Server::announce");
  }

  while (!stop_flag_) {
    GameMessage msg;

    msg.set_msg_seq(0);
    msg.set_sender_id(local_client_->getId());

    msg.mutable_announcement()->set_can_join(
        field_->getFreeSquareCenter().has_value());
    player_mutex_.lock();
    msg.mutable_announcement()->mutable_players()->mutable_players()->CopyFrom(
        current_state_.players().players());
    player_mutex_.unlock();
    msg.mutable_announcement()->mutable_config()->CopyFrom(config_);

    auto size = msg.ByteSize();
    auto buffer = new char[size];
    msg.SerializeToArray(buffer, size);

    sendto(server_socket_, buffer, size, 0, (sockaddr *)&multicast,
           sizeof(sockaddr_in));

    delete[] buffer;

    std::this_thread::sleep_for(seconds(1));
  }
}

std::vector<std::pair<IPv4Address, GameMessage>> Server::getGameMessages() {
  std::map<IPv4Address, GameMessage> steer_msgs;
  std::vector<std::pair<IPv4Address, GameMessage>> msgs;

  last_state_update_ = getTime();
  while ((getTime() - last_state_update_) <= config_.state_delay_ms()) {
    char buffer[BUFSIZ];
    sockaddr_in client_address{};
    socklen_t client_socklen = sizeof(sockaddr_in);
    ssize_t len = 0;
    if ((len = recvfrom(server_socket_, buffer, BUFSIZ, 0,
                        (sockaddr *)&client_address, &client_socklen)) <= 0) {
      continue;
    }

    GameMessage msg;
    msg.ParseFromArray(buffer, len);

    IPv4Address tmp_address(client_address);
    if (!msg.has_steer()) {
      msgs.emplace_back(tmp_address, msg);
    } else {
      steer_msgs.insert_or_assign(tmp_address, msg);
    }
  }

  for (const auto &steer : steer_msgs) {
    msgs.emplace_back(steer.first, steer.second);
  }

  return msgs;
}

std::vector<GameMessage>
Server::step(const std::vector<std::pair<IPv4Address, GameMessage>> &msgs) {
  std::vector<GameMessage> ack_messages;
  std::vector<int32_t> steered_clients;

  player_mutex_.lock();
  auto increased_players = Snake::foodInteraction(current_state_);
  player_mutex_.unlock();

  // process messages from nodes
  for (auto message : msgs) {
    GameMessage ack;
    ack.mutable_ack();
    ack.set_sender_id(local_client_->getId());
    ack.set_receiver_id(message.second.sender_id());
    ack.set_msg_seq(message.second.msg_seq());

    switch (message.second.Type_case()) {
    case GameMessage::kPing:
      msg_queues_[message.second.sender_id()].second = getTime();

      ack_messages.push_back(ack);
      break;
    case GameMessage::kSteer: {
      auto snake =
          std::find_if(current_state_.mutable_snakes()->begin(),
                       current_state_.mutable_snakes()->end(),
                       [message](const GameState_Snake &s) {
                         return message.second.sender_id() == s.player_id();
                       });
      if (snake != current_state_.mutable_snakes()->end()) {
        if (snake->state() !=
            GameState_Snake_SnakeState::GameState_Snake_SnakeState_ZOMBIE) {
          bool increase =
              std::find(increased_players.begin(), increased_players.end(),
                        message.second.sender_id()) != increased_players.end();
          Snake::move(*snake, message.second.steer().direction(), increase);
          steered_clients.push_back(message.second.sender_id());
        }
      }
      msg_queues_[message.second.sender_id()].second = getTime();
      ack_messages.push_back(ack);
      break;
    }
    case GameMessage::kAck: {
      while (!msg_queues_[message.second.sender_id()].first.empty() &&
             msg_queues_[message.second.sender_id()].first.front().msg_seq() <=
                 message.second.msg_seq()) {
        msg_queues_[message.second.sender_id()].first.pop();
        msg_queues_[message.second.sender_id()].second = getTime();
      }
      break;
    }
    case GameMessage::kJoin: {
      auto status =
          createNewPlayer(message.second.join().name(), message.first,
                          message.second.join().player_type(),
                          message.second.join().only_view() ? NodeRole::VIEWER
                                                            : NodeRole::NORMAL);
      if (status.first == -1) {
        ack.mutable_error()->set_error_message(status.second);
        auto buffer = new char[ack.ByteSize()];
        ack.SerializeToArray(buffer, ack.ByteSize());

        sendto(server_socket_, buffer, ack.ByteSize(), 0,
               (sockaddr *)message.first.getSockaddrStruct(),
               sizeof(sockaddr_in));
      } else {
        ack.set_receiver_id(status.first);
        msg_queues_[status.first].second = getTime();

        ack_messages.push_back(ack);
      }
      break;
    }
    case GameMessage::kRoleChange: {
      player_mutex_.lock();
      auto player = std::find_if(
          current_state_.mutable_players()->mutable_players()->begin(),
          current_state_.mutable_players()->mutable_players()->end(),
          [message](const GamePlayer &p) {
            return p.id() == message.second.sender_id();
          });
      player_mutex_.unlock();
      if (player !=
          current_state_.mutable_players()->mutable_players()->end()) {
        player->set_role(NodeRole::VIEWER);
        auto snake =
            std::find_if(current_state_.mutable_snakes()->begin(),
                         current_state_.mutable_snakes()->end(),
                         [message](const GameState_Snake &s) {
                           return s.player_id() == message.second.sender_id();
                         });
        if (snake != current_state_.mutable_snakes()->end()) {
          snake->set_state(
              GameState_Snake_SnakeState::GameState_Snake_SnakeState_ZOMBIE);
        }
      }
      msg_queues_[message.second.sender_id()].second = getTime();

      ack_messages.push_back(ack);
      break;
    }
    case GameMessage::TYPE_NOT_SET:
    default:
      break;
    }
  }

  // move snakes without steer command
  std::sort(steered_clients.begin(), steered_clients.end());
  for (auto &snake : *current_state_.mutable_snakes()) {
    if (!std::binary_search(steered_clients.begin(), steered_clients.end(),
                            snake.player_id())) {
      bool increase =
          std::find(increased_players.begin(), increased_players.end(),
                    snake.player_id()) != increased_players.end();
      Snake::move(snake, snake.head_direction(), increase);
    }
  }

  auto dead_snakes = Snake::snakesInteraction(current_state_);
  auto players = Snake::snakesToFood(current_state_, config_.dead_food_prob(),
                                     dead_snakes);

  for (auto player : players) {
    player_mutex_.lock();
    auto client_iter = std::find_if(
        current_state_.mutable_players()->mutable_players()->begin(),
        current_state_.mutable_players()->mutable_players()->end(),
        [player](const GamePlayer &p) { return p.id() == player; });
    player_mutex_.unlock();
    // if DEPUTY -> VIEWER then look up new DEPUTY
    if (client_iter->role() == NodeRole::DEPUTY) {
      chooseNewDeputy();
    }
    client_iter->set_role(NodeRole::VIEWER);
    auto snake_iter = std::find_if(
        current_state_.mutable_snakes()->begin(),
        current_state_.mutable_snakes()->end(),
        [player](const GameState_Snake &s) { return s.player_id() == player; });
    current_state_.mutable_snakes()->erase(snake_iter);
    if (client_iter->id() == local_client_->getId()) {
      stop(true);
    }
    auto change_role_msg = makeChangeRoleMessage(
        NodeRole::NORMAL, NodeRole::VIEWER, client_iter->id());
    msg_queues_[client_iter->id()].first.push(change_role_msg);
    ack_messages.push_back(change_role_msg);
  }
  genFood();

  return ack_messages;
}

void Server::sendGameMessages(const std::vector<GameMessage> &msgs) {
  for (const auto &msg : msgs) {
    auto msg_size = msg.ByteSize();
    auto buffer = new char[msg_size];
    msg.SerializeToArray(buffer, msg_size);

    sendto(
        server_socket_, buffer, msg_size, 0,
        (sockaddr *)(clients_addresses_[msg.receiver_id()].getSockaddrStruct()),
        sizeof(sockaddr_in));

    delete[] buffer;
  }

  current_state_.set_state_order(current_state_.state_order() + 1);

  GameMessage message;
  message.mutable_state()->mutable_state()->CopyFrom(current_state_);
  message.set_sender_id(local_client_->getId());
  message.set_msg_seq(msg_seq_++);

  auto message_len = message.ByteSize();
  auto buffer = new char[message_len];
  message.SerializeToArray(buffer, message_len);

  for (auto client : clients_addresses_) {
    sendto(server_socket_, buffer, message_len, 0,
           (sockaddr *)client.second.getSockaddrStruct(), sizeof(sockaddr_in));
    msg_queues_[client.first].first.push(message);
  }

  delete[] buffer;

  std::vector<int> clients_to_delete;
  // try to resend old messages
  for (auto repeated_msg : msg_queues_) {
    auto delay = (getTime() - repeated_msg.second.second);
    if (delay >= config_.ping_delay_ms() && delay < config_.node_timeout_ms()) {
      if (repeated_msg.second.first.empty()) {
        GameMessage ping;
        ping.set_msg_seq(msg_seq_++);
        ping.set_sender_id(local_client_->getId());
        ping.set_receiver_id(repeated_msg.first);
        ping.mutable_ping();

        message_len = ping.ByteSize();
        buffer = new char[message_len];
        ping.SerializeToArray(buffer, message_len);

        sendto(server_socket_, buffer, message_len, 0,
               (sockaddr *)(clients_addresses_[repeated_msg.first]
                                .getSockaddrStruct()),
               sizeof(sockaddr_in));

        msg_queues_[repeated_msg.first].first.push(ping);

        delete[] buffer;
        continue;
      }
      auto msg = repeated_msg.second.first.front();
      auto msg_len = msg.ByteSize();
      buffer = new char[msg_len];
      msg.SerializeToArray(buffer, msg_len);

      sendto(server_socket_, buffer, msg_len, 0,
             (sockaddr *)clients_addresses_[repeated_msg.first]
                 .getSockaddrStruct(),
             sizeof(sockaddr_in));

      delete[] buffer;
    } else if (delay >= config_.node_timeout_ms()) {
      player_mutex_.lock();
      auto client = std::find_if(
          current_state_.mutable_players()->mutable_players()->begin(),
          current_state_.mutable_players()->mutable_players()->end(),
          [repeated_msg](const GamePlayer &p) {
            return p.id() == repeated_msg.first;
          });
      player_mutex_.unlock();
      if (client->role() == NodeRole::DEPUTY) {
        chooseNewDeputy();
      }
      if (client->role() != NodeRole::VIEWER) {
        client->set_role(NodeRole::VIEWER);
        auto snake = std::find_if(current_state_.mutable_snakes()->begin(),
                                  current_state_.mutable_snakes()->end(),
                                  [client](const GameState_Snake &s) {
                                    return client->id() == s.player_id();
                                  });
        snake->set_state(GameState_Snake_SnakeState_ZOMBIE);
      } else {
        // delete dead VIEWERS
        clients_to_delete.push_back(client->id());
        clients_addresses_.erase(client->id());
        current_state_.mutable_players()->mutable_players()->erase(client);
      }
    }
  }
  for (auto id : clients_to_delete) {
    msg_queues_.erase(id);
  }
  if (no_deputy_flag_) {
    chooseNewDeputy();
  }
}

void Server::run() {
  announcement_thread_ = std::thread(&Server::announce, this);
  while (!stop_flag_) {
    auto messages = getGameMessages();
    auto responses = step(messages);
    sendGameMessages(responses);
  }
  announcement_thread_.join();
}

Server::Server(const GameState &state, const GameConfig &config, int socket)
    : local_client_(Client::getInstance()), field_(GameField::getInstance()),
      current_state_(state), config_(config), server_socket_(socket),
      last_state_update_(getTime()),
      max_player_id_(Client::getInstance()->getId()), stop_flag_(false),
      msg_seq_(0), no_deputy_flag_(true) {

  sockaddr_in server_sockaddr{};
  socklen_t server_sockaddr_len = sizeof(sockaddr_in);
  if (getsockname(server_socket_, (sockaddr *)&server_sockaddr,
                  &server_sockaddr_len) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Server::Server");
  }
  server_address_ = IPv4Address(server_sockaddr);

  // get maximal id of the players
  for (const auto &player : current_state_.players().players()) {
    if (player.id() > max_player_id_) {
      max_player_id_ = player.id();
    }
  }
  max_player_id_++;

  // set local node as a MASTER
  auto old_deputy = std::find_if(
      current_state_.mutable_players()->mutable_players()->begin(),
      current_state_.mutable_players()->mutable_players()->end(),
      [this](const GamePlayer &p) { return p.id() == local_client_->getId(); });
  if (old_deputy ==
      current_state_.mutable_players()->mutable_players()->end()) {
    throw std::logic_error("Server::Server: no such player!");
  }
  old_deputy->set_role(NodeRole::MASTER);

  // get nodes addresses
  for (const auto &player : current_state_.players().players()) {
    clients_addresses_.insert(std::make_pair(
        player.id(), IPv4Address(player.ip_address(), player.port())));
    msg_queues_[player.id()].second = getTime();
  }

  // set new address for local node
  clients_addresses_[local_client_->getId()] =
      local_client_->getClientAddress();

  // notify all nodes about MASTER changing
  for (auto client : clients_addresses_) {
    auto change_role_msg =
        makeChangeRoleMessage(NodeRole::MASTER, NodeRole::NORMAL, client.first);
    auto msg_size = change_role_msg.ByteSize();
    auto buffer = new char[msg_size];
    change_role_msg.SerializeToArray(buffer, msg_size);

    sendto(server_socket_, buffer, msg_size, 0,
           (sockaddr *)client.second.getSockaddrStruct(), sizeof(sockaddr_in));

    msg_queues_[client.first].first.push(change_role_msg);
    delete[] buffer;
  }

  chooseNewDeputy();

  run_thread_ = std::thread(&Server::run, this);
}

Server *
Server::getInstance(const int32_t &width, const int32_t &height,
                    const int32_t &food_static, const int32_t &food_per_player,
                    const int32_t &state_delay_ms, const float &dead_food_prob,
                    const int32_t &ping_delay_ms,
                    const int32_t &node_timeout_ms, const std::string &name) {
  if (server_ != nullptr) {
    return server_;
  } else {
    return (server_ = new Server(width, height, food_static, food_per_player,
                                 state_delay_ms, dead_food_prob, ping_delay_ms,
                                 node_timeout_ms, name));
  }
}

Server::Server(const int32_t &width, const int32_t &height,
               const int32_t &food_static, const int32_t &food_per_player,
               const int32_t &state_delay_ms, const float &dead_food_prob,
               const int32_t &ping_delay_ms, const int32_t &node_timeout_ms,
               const std::string &name)
    : server_socket_(socket(AF_INET, SOCK_DGRAM, 0)), last_state_update_(0),
      max_player_id_(0), stop_flag_(false), local_client_(nullptr),
      field_(GameField::getInstance(width, height, ping_delay_ms)), msg_seq_(0),
      no_deputy_flag_(true) {
  config_.set_width(width);
  config_.set_height(height);
  config_.set_food_static(food_static);
  config_.set_food_per_player(food_per_player);
  config_.set_state_delay_ms(state_delay_ms);
  config_.set_dead_food_prob(dead_food_prob);
  config_.set_ping_delay_ms(ping_delay_ms);
  config_.set_node_timeout_ms(node_timeout_ms);

  if (server_socket_ < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Server::Server");
  }

  sockaddr_in server_sockaddr{};
  server_sockaddr.sin_family = AF_INET;
  server_sockaddr.sin_port = 0;
  server_sockaddr.sin_addr.s_addr = INADDR_ANY;
  if (bind(server_socket_, (sockaddr *)&server_sockaddr, sizeof(sockaddr_in)) <
      0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Server::Server");
  }

  timeval op_timeout_{};
  op_timeout_.tv_sec = config_.state_delay_ms() / 1000;
  op_timeout_.tv_usec = config_.state_delay_ms() % 1000;
  if (setsockopt(server_socket_, SOL_SOCKET, SO_RCVTIMEO, &op_timeout_,
                 sizeof(op_timeout_)) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Server::Server");
  }

  socklen_t server_sockaddr_len = sizeof(sockaddr_in);
  if (getsockname(server_socket_, (sockaddr *)&server_sockaddr,
                  &server_sockaddr_len) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Server::Server");
  }
  server_address_ = IPv4Address(server_sockaddr);

  GameMessage announce_msg;
  announce_msg.set_msg_seq(0);
  announce_msg.set_sender_id(max_player_id_);
  announce_msg.set_receiver_id(max_player_id_);
  announce_msg.mutable_announcement()->mutable_config()->CopyFrom(config_);
  announce_msg.mutable_announcement()->set_can_join(true);

  // create local node
  local_client_ =
      Client::getInstance(server_address_, name, false, announce_msg, MASTER);
  msg_queues_[max_player_id_].second = getTime();
  auto new_player = createNewPlayer(name, server_address_, PlayerType::HUMAN,
                                    NodeRole::MASTER);
  clients_addresses_[local_client_->getId()] =
      local_client_->getClientAddress();

  current_state_.set_state_order(0);
  current_state_.mutable_config()->CopyFrom(config_);

  run_thread_ = std::thread(&Server::run, this);
}

std::pair<int, std::string> Server::createNewPlayer(const std::string &name,
                                                    const IPv4Address &address,
                                                    const PlayerType &type,
                                                    const NodeRole &role) {
  auto center = field_->getFreeSquareCenter();
  if (center.has_value() || role == NodeRole::VIEWER) {
    player_mutex_.lock();
    auto new_player =
        current_state_.mutable_players()->mutable_players()->Add();
    new_player->set_name(name);
    new_player->set_id(max_player_id_++);
    new_player->set_ip_address(address.getAddress());
    new_player->set_port(address.getPort());
    new_player->set_role(role);
    new_player->set_type(type);
    new_player->set_score(0);
    player_mutex_.unlock();

    if (role != NodeRole::VIEWER) {
      auto snake = Snake::createSnake(center.value(), new_player->id());
      current_state_.mutable_snakes()->Add()->CopyFrom(snake);
    }
    clients_addresses_.insert(std::make_pair(new_player->id(), address));

    return std::make_pair(new_player->id(), "");
  } else {
    return std::make_pair(-1, "No free space on the game field!");
  }
}

Server *Server::getInstance(const GameState &state, const GameConfig &config,
                            int socket) {
  return (server_ == nullptr ? server_ = new Server(state, config, socket)
                             : server_);
}

void Server::chooseNewDeputy() {
  player_mutex_.lock();
  auto new_deputy = std::find_if(
      current_state_.mutable_players()->mutable_players()->begin(),
      current_state_.mutable_players()->mutable_players()->end(),
      [](const GamePlayer &p) { return p.role() == NodeRole::NORMAL; });
  player_mutex_.unlock();
  if (new_deputy !=
      current_state_.mutable_players()->mutable_players()->end()) {
    no_deputy_flag_ = false;
    GameMessage change_role = makeChangeRoleMessage(
        NodeRole::NORMAL, NodeRole::DEPUTY, new_deputy->id());
    new_deputy->set_role(NodeRole::DEPUTY);

    auto change_role_msg_size = change_role.ByteSize();
    auto buffer = new char[change_role_msg_size];
    change_role.SerializeToArray(buffer, change_role_msg_size);

    sendto(server_socket_, buffer, change_role_msg_size, 0,
           (sockaddr *)clients_addresses_[new_deputy->id()].getSockaddrStruct(),
           sizeof(sockaddr_in));

    msg_queues_[new_deputy->id()].first.push(change_role);

    delete[] buffer;
  } else {
    no_deputy_flag_ = true;
  }
}

GameMessage Server::makeChangeRoleMessage(const NodeRole &sender_role,
                                          const NodeRole &receiver_role,
                                          const int32_t &receiver_id) {
  GameMessage change_role;

  change_role.mutable_role_change()->set_sender_role(sender_role);
  change_role.mutable_role_change()->set_receiver_role(receiver_role);
  change_role.set_msg_seq(msg_seq_++);
  change_role.set_sender_id(local_client_->getId());
  change_role.set_receiver_id(receiver_id);

  return change_role;
}

Client *Client::client_ = nullptr;
Server *Client::local_server_ = nullptr;

Client::Client(const IPv4Address &server_address, const std::string &name,
               const bool &only_view, const GameMessage &game_announce,
               const NodeRole &role)
    : stop_flag_(false), local_socket_(socket(AF_INET, SOCK_DGRAM, 0)),
      server_address_(server_address), deputy_address_(server_address),
      msg_seq_(0), player_name_(name), only_view_(only_view),
      config_(game_announce.announcement().config()), role_(role),
      server_id_(game_announce.sender_id()), client_id_(0),
      server_last_ping_(Server::getTime()),
      client_last_ping_(Server::getTime()) {

  field_ = GameField::getInstance(config_.width(), config_.height(),
                                  config_.ping_delay_ms());
  if (local_socket_ < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::Client");
  }

  sockaddr_in local_sockaddr{};
  local_sockaddr.sin_family = AF_INET;
  local_sockaddr.sin_port = 0;
  local_sockaddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(local_socket_, (sockaddr *)&local_sockaddr, sizeof(sockaddr_in)) <
      0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::Client");
  }
  socklen_t local_sockaddr_len = sizeof(sockaddr_in);
  if (getsockname(local_socket_, (sockaddr *)&local_sockaddr,
                  &local_sockaddr_len) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::Client");
  }
  client_address_ = IPv4Address(local_sockaddr);

  timeval op_timeout_{};
  op_timeout_.tv_sec = config_.ping_delay_ms() / 1000;
  op_timeout_.tv_usec = config_.ping_delay_ms() % 1000;
  if (setsockopt(local_socket_, SOL_SOCKET, SO_RCVTIMEO, &op_timeout_,
                 sizeof(op_timeout_)) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::Client");
  }

  // set deputy (deputy == master as default)
  auto players = game_announce.announcement().players().players();

  auto deputy = players.end();
  for (auto player = players.begin(); player != players.end(); player++) {
    if (player->role() == NodeRole::DEPUTY) {
      deputy = player;
      break;
    } else if (player->role() == NodeRole::MASTER) {
      if (role_ != MASTER) {
        server_address_ =
            IPv4Address(server_address_.getAddress(), player->port());
      }
      deputy_address_ = IPv4Address(player->ip_address(), player->port());
    }
  }
  if (deputy != players.end()) {
    deputy_address_ = IPv4Address(deputy->ip_address(), deputy->port());
  }
}

std::pair<GameMessage, int32_t> Client::readMove() {
  auto command = field_->getCommand();
  if (only_view_ && command != 'q') {
    command = ERR;
  }

  GameMessage msg;
  msg.set_sender_id(client_id_);
  msg.set_receiver_id(server_id_);
  msg.set_msg_seq(msg_seq_++);
  auto steer_msg = msg.mutable_steer();

  switch (command) {
  case KEY_UP:
    steer_msg->set_direction(Direction::UP);
    break;
  case KEY_DOWN:
    steer_msg->set_direction(Direction::DOWN);
    break;
  case KEY_LEFT:
    steer_msg->set_direction(Direction::LEFT);
    break;
  case KEY_RIGHT:
    steer_msg->set_direction(Direction::RIGHT);
    break;
  case 'q':
  default:
    break;
  }

  return std::make_pair(msg, command);
}

void Client::sendMove() {
  while (!stop_flag_) {
    auto steer_msg = readMove();

    if (steer_msg.second == 'q') {
      stop_flag_ = true;
      if (local_server_ != nullptr) {
        local_server_->stop(true);
      }
      continue;
    } else if (steer_msg.second != KEY_UP && steer_msg.second != KEY_DOWN &&
               steer_msg.second != KEY_LEFT && steer_msg.second != KEY_RIGHT) {
      auto delay = (Server::getTime() - client_last_ping_);
      if (delay >= config_.ping_delay_ms() &&
          delay < config_.node_timeout_ms()) {
        message_mutex_.lock();
        if (messages_.empty()) {
          steer_msg.first.clear_steer();
          steer_msg.first.mutable_ping();

          auto steer_msg_size = steer_msg.first.ping().ByteSize();
          auto buffer = new char[steer_msg_size];
          steer_msg.first.SerializeToArray(buffer, steer_msg_size);
          sendto(local_socket_, buffer, steer_msg_size, 0,
                 (sockaddr *)server_address_.getSockaddrStruct(),
                 sizeof(sockaddr_in));

          messages_.push(steer_msg.first);

          delete[] buffer;
        } else {
          auto repeated_msg = messages_.front();
          auto repeated_msg_size = repeated_msg.ByteSize();
          auto buffer = new char[repeated_msg_size];
          repeated_msg.SerializeToArray(buffer, repeated_msg_size);

          sendto(local_socket_, buffer, repeated_msg_size, 0,
                 (sockaddr *)server_address_.getSockaddrStruct(),
                 sizeof(sockaddr_in));

          delete[] buffer;
        }

        client_last_ping_ = Server::getTime();
        message_mutex_.unlock();
      } else if (delay >= config_.node_timeout_ms()) {
        stop_flag_ = true;
        if (local_server_ != nullptr) {
          local_server_->stop(true);
        }
      }
      continue;
    }

    auto steer_msg_size = steer_msg.first.ByteSize();
    auto buffer = new char[steer_msg_size];
    steer_msg.first.SerializeToArray(buffer, steer_msg_size);

    sendto(local_socket_, buffer, steer_msg_size, 0,
           (sockaddr *)server_address_.getSockaddrStruct(),
           sizeof(sockaddr_in));

    message_mutex_.lock();
    messages_.push(steer_msg.first);
    message_mutex_.unlock();

    delete[] buffer;

    client_last_ping_ = Server::getTime();
  }
}

bool Client::registration() {
  GameMessage msg;
  auto join_msg = msg.mutable_join();
  msg.set_msg_seq(msg_seq_++);
  join_msg->set_name(player_name_);
  join_msg->set_only_view(only_view_);
  join_msg->set_player_type(PlayerType::HUMAN);

  auto msg_size = msg.ByteSize();
  auto buffer = new char[msg_size];
  msg.SerializeToArray(buffer, msg_size);
  if (sendto(local_socket_, buffer, msg_size, MSG_NOSIGNAL,
             (sockaddr *)server_address_.getSockaddrStruct(),
             sizeof(sockaddr_in)) <= 0) {
    delete[] buffer;
    return false;
  }
  client_last_ping_ = Server::getTime();
  delete[] buffer;

  buffer = new char[BUFSIZ];
  ssize_t size = 0;
  // endless waiting of the answer
  while ((size = recv(local_socket_, buffer, BUFSIZ, 0)) <= 0) {
  }
  server_last_ping_ = Server::getTime();

  GameMessage ack;
  ack.ParseFromArray(buffer, size);
  if (!ack.has_ack()) {
    field_->printErrorMessage(ack.error().error_message());
    std::this_thread::sleep_for(seconds(ERROR_DELAY));
    delete[] buffer;
    return false;
  }

  client_id_ = ack.receiver_id();
  delete[] buffer;

  return true;
}

void Client::receiveGameState() {
  while (!stop_flag_) {
    if ((Server::getTime() - server_last_ping_) >= config_.node_timeout_ms()) {
      if (role_ != NodeRole::DEPUTY) {
        if (server_address_ == deputy_address_) {
          stop_flag_ = true;
          continue;
        }
        server_address_ = deputy_address_;
      } else if (role_ == NodeRole::DEPUTY) {
        becomeMaster();
      }

      server_last_ping_ = Server::getTime();
    }
    char buffer[BUFSIZ];
    ssize_t size;
    sockaddr_in sender{};
    socklen_t sender_structure_len = sizeof(sockaddr_in);

    if ((size = recvfrom(local_socket_, buffer, BUFSIZ, 0, (sockaddr *)&sender,
                         &sender_structure_len)) <= 0) {
      continue;
    } else {
      server_last_ping_ = Server::getTime();

      GameMessage msg;
      msg.ParseFromArray(buffer, size);

      GameMessage ack;
      ack.set_msg_seq(msg.msg_seq());
      ack.set_sender_id(client_id_);
      ack.set_receiver_id(server_id_);
      ack.mutable_ack();

      switch (msg.Type_case()) {
      case GameMessage::kPing:
        break;
      case GameMessage::kAck: {
        client_id_ = msg.receiver_id();
        message_mutex_.lock();
        while (!messages_.empty()) {
          if (messages_.front().msg_seq() <= msg.msg_seq()) {
            messages_.pop();
          }
        }
        message_mutex_.unlock();
        break;
      }
      case GameMessage::kState:
        updateGameField(msg.state().state());
        break;
      case GameMessage::kRoleChange: {
        auto receiver_role = msg.role_change().receiver_role();
        auto sender_role = msg.role_change().sender_role();
        if (receiver_role == NodeRole::DEPUTY) {
          role_ = NodeRole::DEPUTY;
        } else if (receiver_role == NodeRole::VIEWER) {
          only_view_ = true;
          role_ = NodeRole::VIEWER;
        } else if (receiver_role == NodeRole::MASTER) {
          becomeMaster();
        }
        if (sender_role == NodeRole::MASTER) {
          server_id_ = msg.sender_id();
          server_address_ = IPv4Address(sender);
          deputy_address_ = server_address_;
        }
        break;
      }
      default:
        break;
      }

      if (!msg.has_ack()) {
        auto ack_size = ack.ByteSize();
        auto ack_buffer = new char[ack_size];
        ack.SerializeToArray(ack_buffer, ack_size);

        sendto(local_socket_, ack_buffer, ack_size, 0,
               (sockaddr *)server_address_.getSockaddrStruct(),
               sizeof(sockaddr_in));

        client_last_ping_ = Server::getTime();
        delete[] ack_buffer;
      }
    }
  }
}

void Client::updateGameField(const GameState &state) {
  if (state_.state_order() >= state.state_order()) {
    return;
  }
  state_ = state;

  field_->clearField();
  for (const auto &food : state.foods()) {
    field_->showPoint(Coord::normalize(food), Coord::getFoodSymbol());
  }
  for (const auto &snake : state.snakes()) {
    Snake::updateGameField(field_, snake, snake.player_id() == client_id_);
  }
  size_t i = 0;
  deputy_address_ = server_address_;
  for (const auto &player : state.players().players()) {
    if (player.role() != NodeRole::VIEWER) {
      field_->printOutGameFieldText(
          player.name() + ": " + std::to_string(player.score()) + " " +
              roleToString(player.role()) +
              (player.id() == client_id_ ? " L" : " R"),
          i++);
      if (player.role() == NodeRole::DEPUTY &&
          player.ip_address() + ":" + std::to_string(player.port()) !=
              deputy_address_.getFullAddress()) {
        deputy_address_ = IPv4Address(player.ip_address(), player.port());
      }
    }
  }
  field_->refreshField();
}

void Client::run() {
  if (role_ != MASTER) {
    if (!registration()) {
      return;
    }
  }

  sendMove_thread_ = std::thread(&Client::sendMove, this);
  receiveGameState_thread_ = std::thread(&Client::receiveGameState, this);

  sendMove_thread_.join();
  receiveGameState_thread_.join();

  field_->printErrorMessage("GAME OVER");
  std::this_thread::sleep_for(seconds(ERROR_DELAY));
}

Client *Client::getInstance(const IPv4Address &server_address,
                            const std::string &name, const bool &only_view,
                            const GameMessage &game_announce,
                            const NodeRole &role) {
  return client_ == nullptr
             ? client_ = new Client(server_address, name, only_view,
                                    game_announce, role)
             : client_;
}

std::string Client::roleToString(const NodeRole &role) {
  switch (role) {
  case NORMAL:
    return "N";
  case MASTER:
    return "M";
  case DEPUTY:
    return "D";
  }
}

void Client::becomeMaster() {
  int old_socket = local_socket_;
  local_socket_ = socket(AF_INET, SOCK_DGRAM, 0);

  if (local_socket_ < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::becomeMaster");
  }

  sockaddr_in local_sockaddr{};
  local_sockaddr.sin_family = AF_INET;
  local_sockaddr.sin_port = 0;
  local_sockaddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(local_socket_, (sockaddr *)&local_sockaddr, sizeof(sockaddr_in)) <
      0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::becomeMaster");
  }

  socklen_t local_sockaddr_len = sizeof(sockaddr_in);
  if (getsockname(local_socket_, (sockaddr *)&local_sockaddr,
                  &local_sockaddr_len) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::becomeMaster");
  }
  client_address_ = IPv4Address(local_sockaddr);

  timeval op_timeout_{};
  op_timeout_.tv_sec = config_.ping_delay_ms() / 1000;
  op_timeout_.tv_usec = config_.ping_delay_ms() % 1000;
  if (setsockopt(local_socket_, SOL_SOCKET, SO_RCVTIMEO, &op_timeout_,
                 sizeof(op_timeout_)) < 0) {
    throw std::system_error(std::error_code(errno, std::system_category()),
                            "Client::becomeMaster");
  }

  local_server_ = Server::getInstance(state_, config_, old_socket);
  role_ = MASTER;
  server_address_ = local_server_->getServerAddress();
  server_id_ = client_id_;
}