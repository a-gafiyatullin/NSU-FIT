#include "Snake.h"

#include <utility>

Snake::Snake(const Point &head_point, std::shared_ptr<GameField> field)
    : field(std::move(field)) {
  points.push_back(head_point);
  points.back().normalize();

  srand(time(nullptr));
  Point new_point_shift;
  switch (rand() % 4) {
  case 0:
    new_point_shift = points.back().getLeftNeighbour() - points.back();
    curr_direction = RIGHT;
    break;
  case 1:
    new_point_shift = points.back().getRightNeighbour() - points.back();
    curr_direction = LEFT;
    break;
  case 2:
    new_point_shift = points.back().getUpperNeighbour() - points.back();
    curr_direction = DOWN;
    break;
  case 3:
    new_point_shift = points.back().getLowerNeighbour() - points.back();
    curr_direction = UP;
    break;
  }
  points.push_back(new_point_shift);
}

Snake::Snake(std::vector<Point> points, std::shared_ptr<GameField> field,
             const direction &head_direction)
    : points(std::move(points)), field(std::move(field)),
      curr_direction(head_direction) {}

void Snake::updateGameField() const {
  if (points.empty()) {
    return;
  }
  Point prev_point = points.front();
  size_t size = points.size();
  for (size_t i = 1; i < size; i++) {
    field->drawLine(prev_point, points[i]);
    prev_point = prev_point + points[i];
  }
}

bool Snake::moveUp() {
  if(curr_direction == DOWN) {
    return false;
  }
  decreaseSnakeTail();

  points.front() -= Point::getDy();
  switch (curr_direction) {
  case UP:
    points[1] += Point::getDy();
    break;
  case LEFT:
  case RIGHT:
    points.insert(points.begin() + 1, Point::getDy());
    break;
  case DOWN:
    return false;
  }

  if(points.back() == points[points.size() - 2]) {
    points.pop_back();
  }
  curr_direction = UP;
  return true;
}

bool Snake::moveDown() {
  if(curr_direction == UP) {
    return false;
  }
  decreaseSnakeTail();

  points.front() += Point::getDy();
  switch (curr_direction) {
  case UP:
    return false;
  case LEFT:
  case RIGHT:
    points.insert(points.begin() + 1, -Point::getDy());
    break;
  case DOWN:
    points[1] -= Point::getDy();
    break;
  }

  if(points.back() == points[points.size() - 2]) {
    points.pop_back();
  }
  curr_direction = DOWN;
  return true;
}

bool Snake::moveRight() {
  if(curr_direction == LEFT) {
    return false;
  }
  decreaseSnakeTail();

  points.front() += Point::getDx();
  switch (curr_direction) {
  case UP:
  case DOWN:
    points.insert(points.begin() + 1, -Point::getDx());
    break;
  case LEFT:
    return false;
  case RIGHT:
    points[1] -= Point::getDx();
    break;
  }

  if(points.back() == points[points.size() - 2]) {
    points.pop_back();
  }
  curr_direction = RIGHT;
  return true;
}

bool Snake::moveLeft() {
  if(curr_direction == RIGHT) {
    return false;
  }
  decreaseSnakeTail();

  points.front() -= Point::getDx();
  switch (curr_direction) {
  case UP:
  case DOWN:
    points.insert(points.begin() + 1, Point::getDx());
    break;
  case LEFT:
    points[1] += Point::getDx();
    break;
  case RIGHT:
    return false;
  }

  if(points.back() == points[points.size() - 2]) {
    points.pop_back();
  }
  curr_direction = LEFT;
  return true;
}

void Snake::decreaseSnakeTail() {
  if(points.back().getY() == 0) {
    points.back() += (points.back().getX() < 0 ? Point::getDx() : -Point::getDx());
  } else {
    points.back() += (points.back().getY() < 0 ? Point::getDy() : -Point::getDy());
  }
}