#pragma once

#include "../protobuf/snakes.pb.h"

using namespace snakes;

class Coord {
private:
  static int32_t max_x_;
  static int32_t max_y_;

  static unsigned char local_snake_symbol;
  static unsigned char other_snake_symbol;
  static unsigned char food_symbol;

  static GameState_Coord dx;
  static GameState_Coord dy;

  Coord() = default;

public:
  static void initCoordUtils(const int32_t &max_x, const int32_t &max_y);

  static GameState_Coord diff(const GameState_Coord &l,
                              const GameState_Coord &r);
  static GameState_Coord sum(const GameState_Coord &l,
                             const GameState_Coord &r);
  static bool equal(const GameState_Coord &l, const GameState_Coord &r);
  static GameState_Coord negate(const GameState_Coord &coord);

  inline static unsigned char getLocalSnakeSymbol() {
    return local_snake_symbol;
  }
  inline static unsigned char getOtherSnakeSymbol() {
    return other_snake_symbol;
  }
  inline static void setLocalSnakeSymbol(const unsigned char &symbol) {
    local_snake_symbol = symbol;
  }
  inline static void setOtherSnakeSymbol(const unsigned char &symbol) {
    other_snake_symbol = symbol;
  }
  inline static unsigned char getFoodSymbol() { return food_symbol; }
  inline static void setFoodSymbol(const unsigned char &symbol) {
    food_symbol = symbol;
  }

  inline static int32_t getMaxX() { return max_x_; }
  inline static int32_t getMaxY() { return max_y_; }
  inline static void setMaxX(const int32_t &x) { max_x_ = x; }
  inline static void setMaxY(const int32_t &y) { max_y_ = y; }

  static GameState_Coord getDx();
  static GameState_Coord getDy();

  inline static bool isZero(const GameState_Coord &coord) {
    return (coord.x() == 0 && coord.y() == 0);
  };

  // true if central is between l and r, where l and r are in the rectangle
  static bool isBetween(const GameState_Coord &l,
                        const GameState_Coord &central,
                        const GameState_Coord &r);

  static GameState_Coord getLeftNeighbour(const GameState_Coord &coord);
  static GameState_Coord getRightNeighbour(const GameState_Coord &coord);
  static GameState_Coord getUpperNeighbour(const GameState_Coord &coord);
  static GameState_Coord getLowerNeighbour(const GameState_Coord &coord);

  static GameState_Coord normalize(const GameState_Coord &coord);

  static bool lesser(const GameState_Coord &l, const GameState_Coord &r);
};