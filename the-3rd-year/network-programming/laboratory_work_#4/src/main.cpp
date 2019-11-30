#include "backend/SinglePlayerMode.h"

int main() {
  auto mode = SinglePlayerMode::getInstance(25, 80, 0, 0, 0);
  while(mode->step()) {
  }
}