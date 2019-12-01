#pragma once

#include <cstdint>

class Point {
private:
  int32_t x;
  int32_t y;

  int32_t max_x;
  int32_t max_y;

  unsigned char symbol{};

public:
  Point(const int32_t &x, const int32_t &y, const int32_t &max_x,
        const int32_t &max_y, const unsigned char &symbol);
  Point();
  Point(const int32_t &x, const int32_t &y);

  Point operator-(const Point &other) const;
  Point operator+(const Point &other) const;
  Point &operator=(const Point &other);
  bool operator==(const Point &other) const;
  Point &operator-=(const Point &other);
  Point &operator+=(const Point &other);
  Point &operator-();
  bool operator!=(const Point &other) const;
  Point &normalize();

  [[nodiscard]] inline int32_t getX() const { return x; }
  [[nodiscard]] inline int32_t getY() const { return y; }
  [[nodiscard]] inline unsigned char getSymbol() const { return symbol; }
  [[nodiscard]] inline int32_t getMaxX() const { return max_x; }
  [[nodiscard]] inline int32_t getMaxY() const { return max_y; }
  static Point getDx();
  static Point getDy();
  inline bool isZeroPoint() { return (x == 0 && y == 0); };

  inline void setX(const int32_t x) { this->x = x; }
  inline void setY(const int32_t y) { this->y = y; }

  [[nodiscard]] Point getLeftNeighbour() const;
  [[nodiscard]] Point getRightNeighbour() const;
  [[nodiscard]] Point getUpperNeighbour() const;
  [[nodiscard]] Point getLowerNeighbour() const;
};