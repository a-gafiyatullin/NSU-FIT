#include "Snake.h"

void Snake::updateGameField(GameField *field, const GameState_Snake &snake,
                            const bool &is_local_snake) {
  unsigned char symbol = (is_local_snake ? Coord::getLocalSnakeSymbol()
                                         : Coord::getOtherSnakeSymbol());
  if (snake.points().empty()) {
    return;
  }
  auto prev_point = *(snake.points().begin());
  auto size = snake.points().size();
  for (size_t i = 1; i < size; i++) {
    field->drawLine(prev_point, snake.points().Get(i), symbol);
    prev_point = Coord::sum(prev_point, snake.points().Get(i));
  }
}

void Snake::moveUp(GameState_Snake &snake, const bool &increase) {
  auto points = snake.mutable_points();
  if (snake.head_direction() == DOWN) {
    return;
  }
  // don't reduce snake tail if snake would be increased
  if (!increase) {
    decreaseSnakeTail(snake);
  }

  points->begin()->CopyFrom(
      Coord::normalize(Coord::diff(*points->begin(), Coord::getDy())));
  switch (snake.head_direction()) {
  case UP:
    if (points->size() >= 2) { // snake have head and body
      points->Mutable(1)->CopyFrom(Coord::sum(points->Get(1), Coord::getDy()));
    } else if (points->size() == 1) { // snake have only head
      points->Add()->CopyFrom(Coord::getDy());
    }
    break;
  case LEFT:
  case RIGHT:
    insertPoint(snake, 0, Coord::getDy());
    break;
  }
  snake.set_head_direction(UP);
}

void Snake::moveDown(GameState_Snake &snake, const bool &increase) {
  auto points = snake.mutable_points();
  if (snake.head_direction() == UP) {
    return;
  }
  if (!increase) {
    decreaseSnakeTail(snake);
  }

  points->begin()->CopyFrom(
      Coord::normalize(Coord::sum(*points->begin(), Coord::getDy())));
  switch (snake.head_direction()) {
  case LEFT:
  case RIGHT:
    insertPoint(snake, 0, Coord::negate(Coord::getDy()));
    break;
  case DOWN:
    if (points->size() >= 2) {
      points->Mutable(1)->CopyFrom(Coord::diff(points->Get(1), Coord::getDy()));
    } else if (points->size() == 1) {
      points->Add()->CopyFrom(Coord::negate(Coord::getDy()));
    }
    break;
  }

  snake.set_head_direction(DOWN);
}

void Snake::moveRight(GameState_Snake &snake, const bool &increase) {
  auto points = snake.mutable_points();
  if (snake.head_direction() == LEFT) {
    return;
  }
  if (!increase) {
    decreaseSnakeTail(snake);
  }

  points->begin()->CopyFrom(
      Coord::normalize(Coord::sum(*points->begin(), Coord::getDx())));
  switch (snake.head_direction()) {
  case UP:
  case DOWN:
    insertPoint(snake, 0, Coord::negate(Coord::getDx()));
    break;
  case RIGHT:
    if (points->size() >= 2) {
      points->Mutable(1)->CopyFrom(Coord::diff(points->Get(1), Coord::getDx()));
    } else if (points->size() == 1) {
      points->Add()->CopyFrom(Coord::negate(Coord::getDx()));
    }
    break;
  }

  snake.set_head_direction(RIGHT);
}

void Snake::moveLeft(GameState_Snake &snake, const bool &increase) {
  auto points = snake.mutable_points();
  if (snake.head_direction() == RIGHT) {
    return;
  }
  if (!increase) {
    decreaseSnakeTail(snake);
  }

  points->begin()->CopyFrom(
      Coord::normalize(Coord::diff(*points->begin(), Coord::getDx())));
  switch (snake.head_direction()) {
  case UP:
  case DOWN:
    insertPoint(snake, 0, Coord::getDx());
    break;
  case LEFT:
    if (points->size() >= 2) {
      points->Mutable(1)->CopyFrom(Coord::sum(points->Get(1), Coord::getDx()));
    } else if (points->size() == 1) {
      points->Add()->CopyFrom(Coord::getDx());
    }
    break;
  }

  snake.set_head_direction(LEFT);
}

void Snake::decreaseSnakeTail(GameState_Snake &snake) {
  if ((snake.mutable_points()->end() - 1)->y() == 0) {
    (snake.mutable_points()->end() - 1)
        ->CopyFrom(Coord::sum(*(snake.mutable_points()->end() - 1),
                              ((snake.mutable_points()->end() - 1)->x() < 0
                                   ? Coord::getDx()
                                   : Coord::negate(Coord::getDx()))));
  } else {
    (snake.mutable_points()->end() - 1)
        ->CopyFrom(Coord::sum(*(snake.mutable_points()->end() - 1),
                              ((snake.mutable_points()->end() - 1)->y() < 0
                                   ? Coord::getDy()
                                   : Coord::negate(Coord::getDy()))));
  }
  if (Coord::isZero(*(snake.mutable_points()->end() - 1))) {
    snake.mutable_points()->RemoveLast();
  }
}

std::vector<int32_t> Snake::foodInteraction(GameState &state) {
  std::vector<GameState_Coord> eaten_food;
  std::vector<int32_t> increased_snakes;

  for (auto &snake : *state.mutable_snakes()) {
    // find possible eaten food
    auto head = *snake.mutable_points()->begin();
    auto curr_food = std::find_if(
        state.mutable_foods()->begin(), state.mutable_foods()->end(),
        [head](const GameState_Coord &coord) {
          return (head.x() == coord.x()) && (head.y() == coord.y());
        });
    if (curr_food == state.mutable_foods()->end()) {
      continue;
    }
    eaten_food.push_back(*curr_food);
    auto player = std::find_if(
        state.mutable_players()->mutable_players()->begin(),
        state.mutable_players()->mutable_players()->end(),
        [snake](const GamePlayer &p) { return p.id() == snake.player_id(); });
    player->set_score(player->score() + 1);

    increased_snakes.push_back(player->id());
  }
  // remove duplicates
  std::sort(eaten_food.begin(), eaten_food.end(), Coord::lesser);
  eaten_food.erase(
      std::unique(eaten_food.begin(), eaten_food.end(), Coord::equal),
      eaten_food.end());

  for (const auto &i : eaten_food) {
    state.mutable_foods()->erase(std::find_if(
        state.mutable_foods()->begin(), state.mutable_foods()->end(),
        [i](const GameState_Coord &coord) {
          return (i.x() == coord.x()) && (i.y() == coord.y());
        }));
  }

  return increased_snakes;
}

std::vector<int> Snake::snakesInteraction(GameState &state) {
  std::vector<int> dead_snakes;

  // each snake with each snake
  for (size_t i = 0; i < state.snakes().size(); i++) {
    for (size_t k = 0; k < state.snakes().size(); k++) {
      auto head = *(state.snakes().Get(i).points().begin());
      auto next_point = Coord::sum(head, state.snakes().Get(i).points().Get(1));
      auto other_snake = state.snakes().Get(k);
      auto other_curr_point = *(other_snake.points().begin());

      for (int j = 1; j < other_snake.points().size(); j++) {
        auto other_next_point =
            Coord::sum(other_curr_point, other_snake.points().Get(j));
        // case of the different snakes
        bool diff_snakes_interaction =
            (i != k &&
             Coord::isBetween(other_curr_point, head, other_next_point));
        // case of the snake self-interaction
        bool self_interaction = false;
        if (!diff_snakes_interaction && i == k) {
          if (!Coord::equal(other_curr_point, head)) {
            if (Coord::isBetween(other_curr_point, head, other_next_point)) {
              self_interaction = true;
            }
          } else {
            auto n_other_next_point = Coord::normalize(other_next_point);
            if (!Coord::equal(n_other_next_point, other_next_point)) {
              if (head.x() == n_other_next_point.x()) {
                if ((other_next_point.y() <= head.y() &&
                     n_other_next_point.y() <= head.y()) ||
                    ((other_next_point.y() >= head.y() &&
                      n_other_next_point.y() >= head.y()))) {
                  self_interaction = true;
                }
              } else if (head.y() == n_other_next_point.y()) {
                if ((other_next_point.x() <= head.x() &&
                     n_other_next_point.x() <= head.x()) ||
                    ((other_next_point.x() >= head.x() &&
                      n_other_next_point.x() >= head.x()))) {
                  self_interaction = true;
                }
              }
            }
          }
        }

        if (diff_snakes_interaction || self_interaction) {
          dead_snakes.push_back(i);
          if (other_snake.player_id() != state.snakes().Get(i).player_id()) {
            auto player = std::find_if(
                state.mutable_players()->mutable_players()->begin(),
                state.mutable_players()->mutable_players()->end(),
                [other_snake](const GamePlayer &p) {
                  return p.id() == other_snake.player_id();
                });
            player->set_score(player->score() + 1);
          }
          break;
        }
        other_curr_point = other_next_point;
      }
    }
  }
  // remove duplicates
  std::sort(dead_snakes.begin(), dead_snakes.end());
  dead_snakes.erase(std::unique(dead_snakes.begin(), dead_snakes.end()),
                    dead_snakes.end());

  return dead_snakes;
}

void Snake::insertPoint(GameState_Snake &snake, const size_t &after_idx,
                        const GameState_Coord &coord) {
  auto points = snake.mutable_points();

  points->Add()->CopyFrom(coord);
  for (int i = points->size() - 1; i > after_idx + 1; i--) {
    points->SwapElements(i, i - 1);
  }
}

GameState_Snake Snake::createSnake(const GameState_Coord &head,
                                   const int32_t &player) {
  GameState_Snake snake;
  snake.set_player_id(player);
  snake.set_state(GameState_Snake_SnakeState::GameState_Snake_SnakeState_ALIVE);

  snake.mutable_points()->Add()->CopyFrom(head);
  srand(time(nullptr));
  GameState_Coord new_point_shift;
  Direction dir = UP;
  switch (rand() % 4) {
  case 0:
    new_point_shift = Coord::diff(Coord::getLeftNeighbour(head), head);
    dir = RIGHT;
    break;
  case 1:
    new_point_shift = Coord::diff(Coord::getRightNeighbour(head), head);
    dir = LEFT;
    break;
  case 2:
    new_point_shift = Coord::diff(Coord::getUpperNeighbour(head), head);
    dir = DOWN;
    break;
  case 3:
    new_point_shift = Coord::diff(Coord::getLowerNeighbour(head), head);
    dir = UP;
    break;
  }
  snake.mutable_points()->Add()->CopyFrom(new_point_shift);
  snake.set_head_direction(dir);

  return snake;
}

std::vector<int32_t>
Snake::snakesToFood(GameState &state, const double &dead_food_prob,
                    const std::vector<int> &dead_snakes_pos) {
  std::vector<int32_t> players;
  srand(time(nullptr));

  // get all points of the snake and randomize possibility of the food creation
  // in this places
  for (const auto &i : dead_snakes_pos) {
    auto snake = state.snakes().begin() + i;

    auto prev_point = *(snake->points().begin());
    auto size = snake->points().size();
    for (size_t j = 1; j < size; j++) {
      auto length = snake->points().Get(j);

      GameState_Coord curr_len;
      curr_len.set_y(0);
      curr_len.set_x(0);
      GameState_Coord dl;

      if (length.x() == 0) {
        dl = (length.y() > 0 ? Coord::getDy() : Coord::negate(Coord::getDy()));
      } else {
        dl = (length.x() > 0 ? Coord::getDx() : Coord::negate(Coord::getDx()));
      }
      for (; !Coord::equal(curr_len, Coord::sum(length, dl));
           curr_len = Coord::sum(curr_len, dl)) {
        auto curr_point = Coord::sum(prev_point, curr_len);
        if ((double)(rand() % 11) / 10.0 <= dead_food_prob) {
          state.mutable_foods()->Add()->CopyFrom(Coord::normalize(curr_point));
        }
      }

      prev_point = Coord::sum(prev_point, length);
    }

    players.push_back(snake->player_id());
  }
  // remove duplicates
  std::sort(state.mutable_foods()->begin(), state.mutable_foods()->end(),
            Coord::lesser);
  state.mutable_foods()->erase(std::unique(state.mutable_foods()->begin(),
                                           state.mutable_foods()->end(),
                                           Coord::equal),
                               state.mutable_foods()->end());

  return players;
}

void Snake::move(GameState_Snake &snake, const Direction &dir,
                 const bool &increase) {
  Direction direction = dir;

  if (direction == getOppositeDirection(snake.head_direction())) {
    direction = snake.head_direction();
  }
  switch (direction) {
  case UP:
    moveUp(snake, increase);
    break;
  case DOWN:
    moveDown(snake, increase);
    break;
  case LEFT:
    moveLeft(snake, increase);
    break;
  case RIGHT:
    moveRight(snake, increase);
    break;
  }
}

Direction Snake::getOppositeDirection(const Direction &dir) {
  switch (dir) {
  case UP:
    return DOWN;
  case DOWN:
    return UP;
  case LEFT:
    return RIGHT;
  case RIGHT:
    return LEFT;
  }
}
