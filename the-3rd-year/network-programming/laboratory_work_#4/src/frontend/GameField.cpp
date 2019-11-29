#include "GameField.h"

std::shared_ptr<GameField> GameField::instance = nullptr;

GameField::GameField(int32_t width, int32_t height) {
    initscr();
    raw();
    noecho();
    wresize(stdscr, width, height);
    box(stdscr, 0, 0);
    refresh();
}

std::shared_ptr<GameField> GameField::getInstance(int32_t width, int32_t height) {
    if(instance == nullptr) {
        instance.reset(new GameField(width, height));
    }
    return instance;
}

GameField::~GameField() {
    endwin();
}

void GameField::destruct() {
    instance = nullptr;
}

void GameField::showPoint(const Point &point) {
  move(point.getX(), point.getY());
  addch(point.getSymbol());
}
