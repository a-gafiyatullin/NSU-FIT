#include "SinglePlayerMode.h"

std::shared_ptr<SinglePlayerMode> SinglePlayerMode::instance = nullptr;

bool SinglePlayerMode::step() {
  field->clearField();
  local_player_snake->updateGameField();
  field->refreshField();
  return getMove();
}

SinglePlayerMode::SinglePlayerMode(const int32_t &width, const int32_t &height,
                                   const int32_t &food_static,
                                   const int32_t &food_per_player,
                                   const int32_t &state_delay_ms)
    : PlayerMode(width, height, food_static, food_per_player, state_delay_ms,
                 0) {
  local_player_snake = std::make_shared<Snake>(
      Point(width / 2, height / 2, width, height, 'o'), field);
}

std::shared_ptr<SinglePlayerMode> SinglePlayerMode::getInstance(
    const int32_t &width, const int32_t &height, const int32_t &food_static,
    const int32_t &food_per_player, const int32_t &state_delay_ms) {
  if (instance == nullptr) {
    instance.reset(new SinglePlayerMode(width, height, food_static,
                                        food_per_player, state_delay_ms));
  }

  return instance;
}
