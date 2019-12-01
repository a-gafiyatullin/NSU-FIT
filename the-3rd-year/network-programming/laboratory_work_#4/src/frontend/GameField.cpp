#include "GameField.h"
std::shared_ptr<GameField> GameField::instance = nullptr;

GameField::GameField(const int32_t &width, const int32_t &height,
                     const int &timeout)
    : field(nullptr) {
  // initialize ncurses
  initscr();
  cbreak();
  noecho();
  refresh();
  curs_set(0);

  // create game field
  field = newwin(height, width, 0, 0);
  if (field == nullptr) {
    throw std::bad_alloc();
  }
  wtimeout(field, timeout);
  keypad(field, TRUE);
  wrefresh(field);
}

std::shared_ptr<GameField> GameField::getInstance(const int32_t &width,
                                                  const int32_t &height,
                                                  const int &timeout) {
  if (instance == nullptr) {
    instance.reset(new GameField(width, height, timeout));
  }
  return instance;
}

GameField::~GameField() {
  delwin(field);
  endwin();
}

void GameField::showPoint(const Point &point) {
  mvwaddch(field, point.getY(), point.getX(), point.getSymbol());
}

void GameField::drawLine(const Point &from, const Point &length) {
  Point curr_len(0, 0, length.getMaxX(), length.getMaxY(), length.getSymbol());
  Point dl;
  if (length.getX() == 0) {
    dl = (length.getY() > 0 ? Point::getDy() : -Point::getDy());
  } else {
    dl = (length.getX() > 0 ? Point::getDx() : -Point::getDx());
  }
  // show all point in the line
  for (; curr_len != (length + dl); curr_len += dl) {
    auto curr_point = from + curr_len;
    showPoint(curr_point.normalize());
  }
}
