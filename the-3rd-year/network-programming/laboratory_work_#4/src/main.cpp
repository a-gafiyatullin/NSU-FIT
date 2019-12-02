#include "backend/SinglePlayerMode.h"

int main() {
  auto mode = SinglePlayerMode::getInstance(80, 25, 1, 1, 100);
  while (mode->step()) {
  }
}