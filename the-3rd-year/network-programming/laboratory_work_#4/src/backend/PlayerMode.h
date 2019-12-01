#pragma once
#include "../frontend/Snake.h"
#include <algorithm>

class PlayerMode {
protected:
  std::shared_ptr<GameField> field;
  std::vector<std::shared_ptr<Snake>> snakes;
  std::shared_ptr<Snake> local_player_snake;
  std::vector<Point> food;

  // game config
  int32_t width;
  int32_t height;
  int32_t food_static;
  int32_t food_per_player;
  int32_t state_delay_ms;
  double dead_food_prob;

  PlayerMode(const int32_t &width, const int32_t &height,
             const int32_t &food_static, const int32_t &food_per_player,
             const int32_t &state_delay_ms, const double &dead_food_prob);
  PlayerMode();

public:
  bool getMove();
  virtual bool step() = 0;
  void genFood();
  void showFood() const;

  virtual ~PlayerMode() = default;
};