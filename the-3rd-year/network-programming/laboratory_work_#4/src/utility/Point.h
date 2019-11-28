#pragma once

#include <cstdint>

class Point {
private:
  int32_t x;
  int32_t y;

  int32_t max_x;
  int32_t max_y;

public:
  Point(const int32_t &x, const int32_t &y, const int32_t &max_x, const int32_t &max_y);

  Point operator-(const Point &other) const;
  Point operator+(const Point &other) const;
  Point &operator=(const Point &other);

  [[nodiscard]] inline int32_t getX() const { return x; }
  [[nodiscard]] inline int32_t getY() const { return y; }

  inline void setX(const int32_t x) { this->x = x; }
  inline void setY(const int32_t y) { this->y = y; }

  [[nodiscard]] Point getLeftNeighbour() const;
  [[nodiscard]] Point getRightNeighbour() const;
  [[nodiscard]] Point getUpperNeighbour() const;
  [[nodiscard]] Point getLowerNeighbour() const;
};