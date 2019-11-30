#include "GameField.h"
std::shared_ptr<GameField> GameField::instance = nullptr;

GameField::GameField(const int32_t &width, const int32_t &height) {
  initscr();
  raw();
  noecho();
  wresize(stdscr, width, height);
  box(stdscr, 0, 0);
  refresh();
}

std::shared_ptr<GameField> GameField::getInstance(const int32_t &width,
                                                  const int32_t &height) {
  if (instance == nullptr) {
    instance.reset(new GameField(width, height));
  }
  return instance;
}

GameField::~GameField() { endwin(); }

void GameField::showPoint(const Point &point) {
  move(point.getX(), point.getY());
  addch(point.getSymbol());
}

void GameField::drawLine(const Point &from, const Point &length) {
  Point curr_len(0, 0, length.getMaxX(), length.getMaxY(), length.getSymbol());
  Point dl;
  if (length.getX() == 0) {
    dl = Point::getDy();
  } else {
    dl = Point::getDx();
  }
  for (; curr_len != length; curr_len += dl) {
    auto curr_point = from + curr_len;
    showPoint(curr_point.normalize());
  }
  refresh();
}
