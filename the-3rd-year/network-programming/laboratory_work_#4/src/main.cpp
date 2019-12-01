#include "backend/SinglePlayerMode.h"

int main() {
  auto mode = SinglePlayerMode::getInstance(80, 25, 0, 0, 250);
  while (mode->step()) {
  }
}