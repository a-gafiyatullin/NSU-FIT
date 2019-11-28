#include "Point.h"

Point::Point(const int32_t &x, const int32_t &y, const int32_t &max_x, const int32_t &max_y)
: x(x)
, y(y)
, max_x(max_x)
, max_y(max_y) { }

Point Point::operator-(const Point &other) const {
  return Point((x - other.x) % max_x, (y - other.y) % max_y, max_x, max_y);
}

Point Point::operator+(const Point &other) const {
  return Point((x + other.x) % max_x, (y + other.y) % max_y, max_x, max_y);
}

Point &Point::operator=(const Point &other) {
  if(this == &other) {
    return *this;
  }
  x = other.x;
  y = other.y;
}

Point Point::getLeftNeighbour() const {
  return *this - Point(1, 0, max_x, max_y);
}

Point Point::getRightNeighbour() const {
  return *this + Point(1, 0, max_x, max_y);
}

Point Point::getUpperNeighbour() const {
  return *this + Point(0, 1, max_x, max_y);
}

Point Point::getLowerNeighbour() const {
  return *this - Point(0, 1, max_x, max_y);
}
