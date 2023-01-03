#include "backend/Game.h"
#include <iostream>
#include <tinyxml.h>

#define SECOND 1000

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "usage: snake [config.xml]" << std::endl;
    return -1;
  }

  bool is_master = false;

  // MASTER attributes
  int32_t width = 40;
  int32_t height = 30;
  int32_t food_static = 1;
  float food_per_player = 1;
  int32_t state_delay_ms = 1000;
  float dead_food_prob = 0.1;
  int32_t ping_delay_ms = 100;
  int32_t node_timeout_ms = 800;

  // NORMAL node attributes
  std::string name;
  bool only_view = false;

  TiXmlDocument config(argv[1]);
  config.LoadFile();

  auto attr = config.RootElement();
  if (attr != nullptr) {
    auto master_attr = attr->FirstChildElement("master");
    if (master_attr != nullptr) {
      is_master = true;
      auto width_attr = master_attr->FirstChildElement("width");
      if (width_attr != nullptr) {
        width = atoi(width_attr->GetText());
      }
      auto height_attr = master_attr->FirstChildElement("height");
      if (height_attr != nullptr) {
        height = atoi(height_attr->GetText());
      }
      auto food_static_attr = master_attr->FirstChildElement("food_static");
      if (food_static_attr != nullptr) {
        food_static = atoi(food_static_attr->GetText());
      }
      auto food_per_player_attr =
          master_attr->FirstChildElement("food_per_player");
      if (food_per_player_attr != nullptr) {
        food_per_player = atof(food_per_player_attr->GetText());
      }
      auto state_delay_ms_attr =
          master_attr->FirstChildElement("state_delay_ms");
      if (state_delay_ms_attr != nullptr) {
        state_delay_ms = atoi(state_delay_ms_attr->GetText());
      }
      auto dead_food_prob_attr =
          master_attr->FirstChildElement("dead_food_prob");
      if (dead_food_prob_attr != nullptr) {
        dead_food_prob = atof(dead_food_prob_attr->GetText());
      }
      auto ping_delay_ms_attr = master_attr->FirstChildElement("ping_delay_ms");
      if (ping_delay_ms_attr != nullptr) {
        ping_delay_ms = atoi(ping_delay_ms_attr->GetText());
      }
      auto node_timeout_ms_attr =
          master_attr->FirstChildElement("node_timeout_ms");
      if (node_timeout_ms_attr != nullptr) {
        node_timeout_ms = atoi(node_timeout_ms_attr->GetText());
      }
    }
    auto node_attr = attr->FirstChildElement("node");
    if (node_attr != nullptr) {
      auto name_attr = node_attr->FirstChildElement("name");
      if (name_attr != nullptr) {
        name = std::string(name_attr->GetText());
      }
      auto only_view_attr = node_attr->FirstChildElement("only_view");
      if (only_view_attr != nullptr) {
        only_view = std::string(only_view_attr->GetText()) == "true";
      }
    }
  }

  initscr();

  if (!is_master) {
    int init_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (init_socket < 0) {
      throw std::system_error(std::error_code(errno, std::system_category()),
                              "main::socket");
    }

    // allow reuse port and address for multicast
    int reuse = 1;
    if (setsockopt(init_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse,
                   sizeof(reuse)) < 0) {
      throw std::system_error(std::error_code(errno, std::system_category()),
                              "main::setsockopt");
    }

    sockaddr_in init_sockaddr{};
    init_sockaddr.sin_family = AF_INET;
    init_sockaddr.sin_port = htons(MULTICAST_PORT);
    init_sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(init_socket, (sockaddr *)&init_sockaddr, sizeof(sockaddr)) < 0) {
      throw std::system_error(std::error_code(errno, std::system_category()),
                              "main::bind");
    }

    // set multicast address receiving
    struct ip_mreq multicast {};
    multicast.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDRESS);
    multicast.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(init_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, &multicast,
                   sizeof(multicast)) < 0) {
      throw std::system_error(std::error_code(errno, std::system_category()),
                              "main::setsockopt");
    }

    timeval op_timeout_{};
    op_timeout_.tv_sec = 1;
    op_timeout_.tv_usec = 0;
    if (setsockopt(init_socket, SOL_SOCKET, SO_RCVTIMEO, &op_timeout_,
                   sizeof(op_timeout_)) < 0) {
      throw std::system_error(std::error_code(errno, std::system_category()),
                              "main::setsockopt");
    }

    // get and show multicast messages
    timeout(SECOND / 4);
    std::map<IPv4Address, GameMessage> announcements;
    char ch;
    while (true) {
      if (isdigit(ch = getch())) {
        if ((ch - '0') < announcements.size()) {
          break;
        }
      } else if (ch == 'q' || ch == 'Q') {
        endwin();
        return 0;
      }

      announcements.clear();
      int64_t last_wait = Server::getTime();
      while ((Server::getTime() - last_wait) < SECOND) {
        char buffer[BUFSIZ];
        sockaddr_in server{};
        socklen_t server_sockaddr_len = sizeof(sockaddr_in);
        int len = recvfrom(init_socket, buffer, BUFSIZ, 0, (sockaddr *)&server,
                           &server_sockaddr_len);
        if (len < 0) {
          continue;
        }
        if (announcements.find(IPv4Address(server)) == announcements.end()) {
          GameMessage announce;
          announce.ParseFromArray(buffer, len);
          announcements.insert(std::make_pair(IPv4Address(server), announce));
        }
      }

      // print 10 announced games
      clear();
      int i = 0;
      int win_width, win_height;
      getmaxyx(stdscr, win_height, win_width);
      mvaddstr(0, win_width / 2.5, "GAMES\n");
      mvaddstr(1, win_width / 4, "Enter number of the game: (0-9)\n");
      mvaddstr(2, win_width / 4, "\n");
      for (const auto &game : announcements) {
        mvaddstr(i + 3, win_width / 6,
                 (std::to_string(i) + ") Players amount: " +
                  std::to_string(
                      game.second.announcement().players().players().size()) +
                  ", can join: " +
                  (game.second.announcement().can_join() ? "true, address: "
                                                         : "false, address: ") +
                  game.first.getFullAddress() + '\n')
                     .c_str());
        i++;
        if (i == 10) {
          break;
        }
      }
      refresh();
    }

    clear();
    Client *client = Client::getInstance(
        std::next(announcements.begin(), (ch - '0'))->first, name, only_view,
        std::next(announcements.begin(), (ch - '0'))->second,
        only_view ? NodeRole::VIEWER : NodeRole::NORMAL);
    client->run();
  } else if (is_master) {
    clear();
    Server *server = Server::getInstance(
        width, height, food_static, food_per_player, state_delay_ms,
        dead_food_prob, ping_delay_ms, node_timeout_ms, name);

    Client *client = Client::getInstance();
    client->run();
  }

  endwin();
}