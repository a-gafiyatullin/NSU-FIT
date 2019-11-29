#include "Snake.h"

Snake::Snake(const Point &head_point, std::shared_ptr<GameField> &field)
: field(field) {
  points.push_back(head_point);

  srand(time(nullptr));
  Point new_point_shift;
  switch (rand() % 4) {
  case 0:
    new_point_shift = points.back().getLeftNeighbour() - points.back();
    break;
  case 1:
    new_point_shift = points.back().getRightNeighbour() - points.back();
    break;
  case 2:
    new_point_shift = points.back().getUpperNeighbour() - points.back();
    break;
  case 3:
    new_point_shift = points.back().getLowerNeighbour() - points.back();
    break;
  }
  points.push_back(new_point_shift);
}

Snake::Snake(std::vector<Point> points)
: points(std::move(points))
{}

Snake::Snake(std::vector<Point> points,
             std::shared_ptr<GameField> &field)
: points(std::move(points))
, field(field) {}

void Snake::updateGameField() const {
}
