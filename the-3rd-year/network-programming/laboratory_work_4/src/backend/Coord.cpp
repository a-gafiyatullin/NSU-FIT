#include "Coord.h"

unsigned char Coord::other_snake_symbol = '#';
unsigned char Coord::local_snake_symbol = 'o';
unsigned char Coord::food_symbol = '*';

GameState_Coord Coord::dx;
GameState_Coord Coord::dy;

int32_t Coord::max_x_ = 0;
int32_t Coord::max_y_ = 0;

GameState_Coord Coord::diff(const GameState_Coord &l,
                            const GameState_Coord &r) {
  GameState_Coord coord;
  coord.set_y(l.y() - r.y());
  coord.set_x(l.x() - r.x());
  return coord;
}

GameState_Coord Coord::sum(const GameState_Coord &l, const GameState_Coord &r) {
  GameState_Coord coord;
  coord.set_y(l.y() + r.y());
  coord.set_x(l.x() + r.x());
  return coord;
}

GameState_Coord Coord::getLeftNeighbour(const GameState_Coord &coord) {
  return Coord::diff(coord, Coord::getDx());
}

GameState_Coord Coord::getRightNeighbour(const GameState_Coord &coord) {
  return Coord::sum(coord, Coord::getDx());
}

GameState_Coord Coord::getUpperNeighbour(const GameState_Coord &coord) {
  return Coord::diff(coord, Coord::getDy());
}

GameState_Coord Coord::getLowerNeighbour(const GameState_Coord &coord) {
  return Coord::sum(coord, Coord::getDy());
}

bool Coord::equal(const GameState_Coord &l, const GameState_Coord &r) {
  return l.x() == r.x() && l.y() == r.y();
}

GameState_Coord Coord::getDx() { return dx; }

GameState_Coord Coord::getDy() { return dy; }

bool Coord::isBetween(const GameState_Coord &l, const GameState_Coord &central,
                      const GameState_Coord &r) {
  auto n_l = Coord::normalize(l);
  auto n_r = Coord::normalize(r);
  auto n_central = Coord::normalize(central);

  if (n_l.x() == n_r.x() && n_l.x() == n_central.x()) {
    GameState_Coord higher, lower;
    if (l.y() <= r.y()) {
      higher = l;
      lower = r;
    } else {
      higher = r;
      lower = l;
    }
    GameState_Coord n_higher = (n_l.y() <= n_r.y() ? l : r);

    if (Coord::equal(higher, n_higher)) {
      return n_central.y() >= Coord::normalize(higher).y() &&
             n_central.y() <= Coord::normalize(lower).y();
    } else {
      return n_central.y() <= Coord::normalize(lower).y() ||
             n_central.y() >= Coord::normalize(higher).y();
    }
  } else if (n_l.y() == n_r.y() && n_l.y() == n_central.y()) {
    GameState_Coord lefter, righter;
    if (l.x() <= r.x()) {
      lefter = l;
      righter = r;
    } else {
      lefter = r;
      righter = l;
    }
    GameState_Coord n_lefter = (n_l.x() <= n_r.x() ? l : r);

    if (Coord::equal(lefter, n_lefter)) {
      return n_central.x() <= Coord::normalize(righter).x() &&
             n_central.x() >= Coord::normalize(lefter).x();
    } else {
      return n_central.x() <= Coord::normalize(righter).x() ||
             n_central.x() >= Coord::normalize(lefter).x();
    }
  }

  return false;
}

GameState_Coord Coord::negate(const GameState_Coord &coord) {
  GameState_Coord new_coord;
  new_coord.set_x(-coord.x());
  new_coord.set_y(-coord.y());

  return new_coord;
}

void Coord::initCoordUtils(const int32_t &max_x, const int32_t &max_y) {
  max_x_ = max_x;
  max_y_ = max_y;
  dx.set_x(1);
  dx.set_y(0);
  dy.set_x(0);
  dy.set_y(1);
}

GameState_Coord Coord::normalize(const GameState_Coord &coord) {
  GameState_Coord new_coord;

  new_coord.set_y((coord.y() % max_y_ + max_y_) % max_y_);
  new_coord.set_x((coord.x() % max_x_ + max_x_) % max_x_);

  return new_coord;
}

bool Coord::lesser(const GameState_Coord &l, const GameState_Coord &r) {
  return l.y() < r.y() && l.x() < r.y();
}
