#include "PlayerMode.h"

PlayerMode::PlayerMode(const int32_t &width, const int32_t &height,
                       const int32_t &food_static,
                       const int32_t &food_per_player,
                       const int32_t &state_delay_ms,
                       const double &dead_food_prob)
    : width(width), height(height), food_static(food_static),
      food_per_player(food_per_player), state_delay_ms(state_delay_ms),
      dead_food_prob(dead_food_prob) {
  field = GameField::getInstance(width, height);
}

PlayerMode::PlayerMode() : PlayerMode(40, 30, 1, 1, 1000, 0.1) {}

bool PlayerMode::getMove() {
  auto curr_command = static_cast<commands>(wgetch(stdscr));
  switch (curr_command) {
  case UPc:
    local_player_snake->moveUp();
    break;
  case DOWNc:
    local_player_snake->moveDown();
    break;
  case LEFTc:
    local_player_snake->moveLeft();
    break;
  case RIGHTc:
    local_player_snake->moveRight();
    break;
  case QUITc:
    return false;
  }

  return true;
}