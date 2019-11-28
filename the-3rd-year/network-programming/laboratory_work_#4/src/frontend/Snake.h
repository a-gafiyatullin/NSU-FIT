#pragma once

#include "../utility/Point.h"
#include <memory>
#include <vector>

class Snake {
private:
  std::vector<std::shared_ptr<Point>> points;

public:
  Snake(std::shared_ptr<Point> &head_point);

  void moveRight();
  void moveLeft();
  void moveUp();
  void moveDown();
};

