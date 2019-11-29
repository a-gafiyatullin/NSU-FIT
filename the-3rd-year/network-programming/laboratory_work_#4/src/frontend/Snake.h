#pragma once

#include "../utility/Point.h"
#include <memory>
#include <vector>
#include "GameField.h"

class Snake {
private:
  std::vector<Point> points;
  std::shared_ptr<GameField> field;
public:
  explicit Snake(const Point &head_point, std::shared_ptr<GameField> &field);
  explicit Snake(std::vector<Point> points);
  Snake(std::vector<Point> points, std::shared_ptr<GameField> &field);

  inline void setSnakePoints(const std::vector<Point> &points) { this->points = points; }
  [[nodiscard]] inline std::vector<Point> getSnakePoints() const { return points; }

  void updateGameField() const;
};

