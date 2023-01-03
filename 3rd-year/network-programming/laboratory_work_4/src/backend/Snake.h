#pragma once

#include "../frontend/GameField.h"
#include "Coord.h"
#include <algorithm>

using namespace snakes;

class Snake {
private:
  static void decreaseSnakeTail(GameState_Snake &snake);

  static void insertPoint(GameState_Snake &snake, const size_t &after_idx,
                          const GameState_Coord &coord);

  static void moveUp(GameState_Snake &snake, const bool &increase);
  static void moveDown(GameState_Snake &snake, const bool &increase);
  static void moveRight(GameState_Snake &snake, const bool &increase);
  static void moveLeft(GameState_Snake &snake, const bool &increase);

  Snake() = default;

  static Direction getOppositeDirection(const Direction &dir);

public:
  // return players id with dead snakes
  static std::vector<int32_t>
  snakesToFood(GameState &state, const double &dead_food_prob,
               const std::vector<int> &dead_snakes_pos);

  // return players id who's snakes would be increased
  static std::vector<int32_t> foodInteraction(GameState &state);

  // return dead snakes position in the snakes array
  static std::vector<int> snakesInteraction(GameState &state);

  static GameState_Snake createSnake(const GameState_Coord &head,
                                     const int32_t &player);

  static void move(GameState_Snake &snake, const Direction &dir,
                   const bool &increase);

  static void updateGameField(GameField *field, const GameState_Snake &snake,
                              const bool &is_local_snake);
};