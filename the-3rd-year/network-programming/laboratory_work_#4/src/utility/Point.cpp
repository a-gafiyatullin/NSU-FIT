#include "Point.h"

Point::Point(const int32_t &x, const int32_t &y, const int32_t &max_x,
             const int32_t &max_y, const unsigned char &symbol)
    : x(x), y(y), max_x(max_x), max_y(max_y), symbol(symbol) {}

Point Point::operator-(const Point &other) const {
  return Point(x - other.x, y - other.y, max_x, max_y, symbol);
}

Point Point::operator+(const Point &other) const {
  return Point(x + other.x, y + other.y, max_x, max_y, symbol);
}

Point &Point::operator=(const Point &other) {
  if (this == &other) {
    return *this;
  }
  x = other.x;
  y = other.y;

  return *this;
}

Point Point::getLeftNeighbour() const {
  return (*this - Point(1, 0, max_x, max_y, symbol)).normalize();
}

Point Point::getRightNeighbour() const {
  return (*this + Point(1, 0, max_x, max_y, symbol)).normalize();
}

Point Point::getUpperNeighbour() const {
  return (*this - Point(0, 1, max_x, max_y, symbol)).normalize();
}

Point Point::getLowerNeighbour() const {
  return (*this + Point(0, 1, max_x, max_y, symbol)).normalize();
}

Point::Point() : x(0), y(0), max_x(0), max_y(0), symbol('o') {}

bool Point::operator==(const Point &other) const {
  return x == other.x && y == other.y;
}

Point &Point::operator-=(const Point &other) {
  x -= other.x;
  y -= other.y;

  return *this;
}

Point &Point::operator+=(const Point &other) {
  x += other.x;
  y += other.y;

  return *this;
}

Point::Point(const int32_t &x, const int32_t &y) : Point(x, y, 0, 0, 'o') {}

Point Point::getDx() {
  static Point dx(1, 0);
  return dx;
}

Point Point::getDy() {
  static Point dy(0, 1);
  return dy;
}

Point &Point::operator-() {
  x = -x;
  y = -y;

  return *this;
}

Point &Point::normalize() {
  x = (x + max_x) % max_x;
  y = (y + max_y) % max_y;

  return *this;
}

bool Point::operator!=(const Point &other) const { return !(*this == other); }

bool Point::isBetween(const Point &left, const Point &right) const {
  if (left.x == right.x && left.x == x) {
    return (y >= std::min(left.y, right.y) && y <= std::max(left.y, right.y));
  } else if (left.y == right.y && left.y == y) {
    return (x >= std::min(left.x, right.x) && x <= std::max(left.x, right.x));
  }
  return false;
}
