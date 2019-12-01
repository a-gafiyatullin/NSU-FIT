#pragma once

#include "../utility/Point.h"
#include "GameField.h"
#include <memory>
#include <vector>

enum direction { UP = 1, DOWN, LEFT, RIGHT };

class Snake {
private:
  std::vector<Point> points;
  std::shared_ptr<GameField> field;

  direction curr_direction;
  void decreaseSnakeTail();

public:
  Snake(const Point &head_point, std::shared_ptr<GameField> field);
  Snake(std::vector<Point> points, std::shared_ptr<GameField> field,
        const direction &head_direction);

  inline void setSnakePoints(const std::vector<Point> &points) {
    this->points = points;
  }

  [[nodiscard]] inline std::vector<Point> getSnakePoints() const {
    return points;
  }

  [[nodiscard]] int getPreviousCommand() const;

  bool moveUp();
  bool moveDown();
  bool moveRight();
  bool moveLeft();

  void updateGameField() const;
};
