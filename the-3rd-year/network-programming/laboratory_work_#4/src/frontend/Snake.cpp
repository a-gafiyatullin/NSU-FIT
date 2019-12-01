#include "Snake.h"

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
  if (curr_direction == DOWN) {
    return false;
  }
  decreaseSnakeTail();

  points.front() -= Point::getDy();
  switch (curr_direction) {
  case UP:
    if (points.size() >= 2) { // snake have head and body
      points[1] += Point::getDy();
    } else if (points.size() == 1) { // snake have only head
      points.push_back(Point::getDy());
    }
    break;
  case LEFT:
  case RIGHT:
    points.insert(points.begin() + 1, Point::getDy());
    break;
  case DOWN:
    return false;
  }

  curr_direction = direction::UP;
  return true;
}

bool Snake::moveDown() {
  if (curr_direction == UP) {
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
    if (points.size() >= 2) {
      points[1] -= Point::getDy();
    } else if (points.size() == 1) {
      points.push_back(-Point::getDy());
    }
    break;
  }

  curr_direction = direction::DOWN;
  return true;
}

bool Snake::moveRight() {
  if (curr_direction == LEFT) {
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
    if (points.size() >= 2) {
      points[1] -= Point::getDx();
    } else if (points.size() == 1) {
      points.push_back(-Point::getDx());
    }
    break;
  }

  curr_direction = direction::RIGHT;
  return true;
}

bool Snake::moveLeft() {
  if (curr_direction == RIGHT) {
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
    if (points.size() >= 2) {
      points[1] += Point::getDx();
    } else if (points.size() == 1) {
      points.push_back(Point::getDx());
    }
    break;
  case RIGHT:
    return false;
  }

  curr_direction = direction::LEFT;
  return true;
}

void Snake::decreaseSnakeTail() {
  if (points.back().getY() == 0) {
    points.back() +=
        (points.back().getX() < 0 ? Point::getDx() : -Point::getDx());
  } else {
    points.back() +=
        (points.back().getY() < 0 ? Point::getDy() : -Point::getDy());
  }
  if (points.back().isZeroPoint()) {
    points.pop_back();
  }
}

int Snake::getPreviousCommand() const {
  switch (curr_direction) {
  case UP:
    return KEY_UP;
  case DOWN:
    return KEY_DOWN;
  case RIGHT:
    return KEY_RIGHT;
  case LEFT:
    return KEY_LEFT;
  }
}

bool Snake::foodInteraction(std::vector<Point> &food) {
  points.front().normalize();
  auto curr_food = std::find(food.begin(), food.end(), points.front());
  if (curr_food == food.end()) {
    return false;
  }
  food.erase(curr_food);
  switch (curr_direction) {
  case UP:
    points.front() -= Point::getDy();
    points[1] += Point::getDy();
    break;
  case DOWN:
    points.front() += Point::getDy();
    points[1] -= Point::getDy();
    break;
  case RIGHT:
    points.front() += Point::getDx();
    points[1] -= Point::getDx();
    break;
  case LEFT:
    points.front() -= Point::getDx();
    points[1] += Point::getDx();
    break;
  }

  return true;
}

bool Snake::selfInteraction() const {}
