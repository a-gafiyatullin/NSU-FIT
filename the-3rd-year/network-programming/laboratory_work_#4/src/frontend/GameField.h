#pragma once

#include "../utility/Point.h"
#include <cstdint>
#include <memory>
#include <ncurses.h>

class GameField {
private:
  static std::shared_ptr<GameField> instance;
  GameField(const int32_t &width, const int32_t &height, const int &timeout);
  WINDOW *field;

public:
  static std::shared_ptr<GameField>
  getInstance(const int32_t &width, const int32_t &height, const int &timeout);

  void showPoint(const Point &point);
  void drawLine(const Point &from, const Point &length);
  inline void clearField() { werase(field); }
  inline void refreshField() { wrefresh(field); }
  inline int getCommand() { return wgetch(field); }

  ~GameField();
};
