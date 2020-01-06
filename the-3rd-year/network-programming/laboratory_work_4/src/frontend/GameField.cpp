#include "GameField.h"

GameField *GameField::instance_ = nullptr;

GameField::GameField(const int32_t &width, const int32_t &height,
                     const int &timeout)
    : width_(width), height_(height), field_(nullptr) {
  if (height < 5 || width < 5) {
    throw std::logic_error("GameField::GameField : height < 5 || width < 5!");
  }
  // initialize ncurses
  cbreak();
  noecho();
  refresh();
  curs_set(0);

  // create game field_
  field_ = newwin(height, width + players_info_field_width_, 0, 0);
  if (field_ == nullptr) {
    throw std::bad_alloc();
  }
  wtimeout(field_, timeout);
  keypad(field_, TRUE);
  wrefresh(field_);

  Coord::initCoordUtils(width, height);
  Coord::setMaxX(width);
  Coord::setMaxY(height);

  // initialize map of the free squares centers
  const int32_t dx = 5;
  for (int32_t x = 2; x < width; x += dx) {
    for (int32_t y = 2; y < height; y += dx) {
      GameState_Coord center;
      center.set_x(x);
      center.set_y(y);
      free_squares_.insert(std::make_pair(center, true));
    }
  }

  free_coord_amount_ = width * height;
  free_coord_map_ = new char[free_coord_amount_];
  memset(free_coord_map_, 0, free_coord_amount_ * sizeof(char));
}

GameField *GameField::getInstance(const int32_t &width, const int32_t &height,
                                  const int &timeout) {
  if (instance_ == nullptr) {
    instance_ = new GameField(width, height, timeout);
  }

  return instance_;
}

GameField::~GameField() {
  delwin(field_);
  delete[] free_coord_map_;
}

void GameField::showPoint(const GameState_Coord &point,
                          const unsigned char &symbol) {
  mvwaddch(field_, point.y(), point.x(), symbol);

  free_coord_amount_--;
  free_coord_map_[point.y() * width_ + point.x()] = -1;
  // update info about free 5x5 squares
  int32_t x_square_num = (point.x() - 2) / 5;
  int32_t y_square_num = (point.y() - 2) / 5;
  GameState_Coord center;
  center.set_x(2 + x_square_num * 5);
  center.set_y(2 + y_square_num * 5);
  free_squares_[center] = false;
}

void GameField::drawLine(const GameState_Coord &from,
                         const GameState_Coord &length,
                         const unsigned char &symbol) {
  GameState_Coord curr_len;
  curr_len.set_y(0);
  curr_len.set_x(0);
  GameState_Coord dl;

  if (length.x() == 0) {
    dl = (length.y() > 0 ? Coord::getDy() : Coord::negate(Coord::getDy()));
  } else {
    dl = (length.x() > 0 ? Coord::getDx() : Coord::negate(Coord::getDx()));
  }

  // show all point in the line
  for (; !Coord::equal(curr_len, Coord::sum(length, dl));
       curr_len = Coord::sum(curr_len, dl)) {
    auto curr_point = Coord::sum(from, curr_len);
    showPoint(Coord::normalize(curr_point), symbol);
  }
}

void GameField::clearField() {
  free_coord_amount_ = Coord::getMaxY() * Coord::getMaxX();
  memset(free_coord_map_, 0, free_coord_amount_ * sizeof(char));
  for (auto &coord : free_squares_) {
    coord.second = true;
  }
  werase(field_);
  wborder(field_, '|', '|', '-', '-', '+', '+', '+', '+');
  mvwvline(field_, 1, width_, '|', height_ - 1);
  mvwaddch(field_, 0, width_, '+');
  mvwaddch(field_, height_ - 1, width_, '+');
  mvwaddnstr(field_, 0, Coord::getMaxX() + 10, "Players", sizeof("Players"));
}

std::optional<GameState_Coord> GameField::getFreeSquareCenter() const {
  auto center = std::find_if(
      free_squares_.begin(), free_squares_.end(),
      [](const std::pair<GameState_Coord, bool> &c) { return c.second; });
  if (center == free_squares_.end()) {
    return {};
  } else {
    return center->first;
  }
}

void GameField::printOutGameFieldText(const std::string &text,
                                      const size_t &line_num) {
  mvwaddnstr(field_, line_num + 1, Coord::getMaxX() + 5, text.c_str(),
             text.length());
}

void GameField::printErrorMessage(const std::string &text) {
  wclear(field_);
  mvwaddnstr(field_, Coord::getMaxY() / 2, Coord::getMaxX() / 2, text.c_str(),
             text.length());
  wrefresh(field_);
}