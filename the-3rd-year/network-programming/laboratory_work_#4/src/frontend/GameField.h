#pragma once

#include "../utility/Point.h"
#include <cstdint>
#include <memory>
#include <ncurses.h>

class GameField {
private:
  static std::shared_ptr<GameField> instance;
  GameField(int32_t width, int32_t height);

public:
  static std::shared_ptr<GameField> getInstance(int32_t width, int32_t height);
  static void destruct();

  static void showPoint(const Point &point);

  ~GameField();
};
