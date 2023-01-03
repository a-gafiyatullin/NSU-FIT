#pragma once

#include "../backend/Coord.h"
#include <algorithm>
#include <memory>
#include <ncurses.h>

using namespace snakes;

class GameField {
private:
  struct CoordCompare {
    bool operator()(const GameState_Coord &a, const GameState_Coord &b) const {
      return a.x() < b.x() && a.y() < b.y();
    }
  };

  int32_t width_;
  int32_t height_;

  static GameField *instance_;
  GameField(const int32_t &width, const int32_t &height, const int &timeout);
  WINDOW *field_;
  // true if square or coord is free
  std::map<GameState_Coord, bool, CoordCompare> free_squares_;
  char *free_coord_map_;
  ssize_t free_coord_amount_;

  static const int players_info_field_width_ = 25;

public:
  static GameField *getInstance(const int32_t &width, const int32_t &height,
                                const int &timeout);

  inline static GameField *getInstance() { return instance_; }

  void showPoint(const GameState_Coord &point, const unsigned char &symbol);

  void drawLine(const GameState_Coord &from, const GameState_Coord &length,
                const unsigned char &symbol);

  void clearField();

  inline void refreshField() { wrefresh(field_); }

  inline int getCommand() { return wgetch(field_); }

  [[nodiscard]] std::optional<GameState_Coord> getFreeSquareCenter() const;

  [[nodiscard]] inline size_t getFreeCoordAmount() const {
    return free_coord_amount_;
  }

  [[nodiscard]] bool isFreeCoord(const GameState_Coord &coord) const {
    return free_coord_map_[coord.y() * width_ + coord.x()] == 0;
  }

  void printOutGameFieldText(const std::string &text, const size_t &line_num);

  void printErrorMessage(const std::string &text);

  ~GameField();
};
