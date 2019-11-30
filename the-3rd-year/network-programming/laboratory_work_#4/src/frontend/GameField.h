#pragma once

#include "../utility/Point.h"
#include <cstdint>
#include <memory>
#include <ncurses.h>

class GameField {
private:
  static std::shared_ptr<GameField> instance;
  GameField(const int32_t &width, const int32_t &height);

public:
  static std::shared_ptr<GameField> getInstance(const int32_t &width, const int32_t &height);

  static void showPoint(const Point &point);
  static void drawLine(const Point &from, const Point &length);

  ~GameField();
};
