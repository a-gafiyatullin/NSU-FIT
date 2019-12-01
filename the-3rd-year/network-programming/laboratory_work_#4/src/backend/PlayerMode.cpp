#include "PlayerMode.h"

PlayerMode::PlayerMode(const int32_t &width, const int32_t &height,
                       const int32_t &food_static,
                       const int32_t &food_per_player,
                       const int32_t &state_delay_ms,
                       const double &dead_food_prob)
    : width(width), height(height), food_static(food_static),
      food_per_player(food_per_player), state_delay_ms(state_delay_ms),
      dead_food_prob(dead_food_prob) {
  field = GameField::getInstance(width, height, state_delay_ms);
}

PlayerMode::PlayerMode() : PlayerMode(40, 30, 1, 1, 1000, 0.1) {}

bool PlayerMode::getMove() {
  static int prev_command = local_player_snake->getPreviousCommand();
  int curr_command = field->getCommand();
  if(curr_command == ERR) {
    curr_command = prev_command;
  } else {
    prev_command = curr_command;
  }
  switch (curr_command) {
  case KEY_UP:
    local_player_snake->moveUp();
    break;
  case KEY_DOWN:
    local_player_snake->moveDown();
    break;
  case KEY_LEFT:
    local_player_snake->moveLeft();
    break;
  case KEY_RIGHT:
    local_player_snake->moveRight();
    break;
  default:
    return false;
  }

  return true;
}
