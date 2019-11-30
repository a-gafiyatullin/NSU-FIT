#pragma once
#include "PlayerMode.h"

class SinglePlayerMode : public PlayerMode {
private:
  static std::shared_ptr<SinglePlayerMode> instance;

  SinglePlayerMode() : PlayerMode(){};

  SinglePlayerMode(const int32_t &width, const int32_t &height,
                   const int32_t &food_static, const int32_t &food_per_player,
                   const int32_t &state_delay_ms);

public:
  static std::shared_ptr<SinglePlayerMode>
  getInstance(const int32_t &width, const int32_t &height,
              const int32_t &food_static, const int32_t &food_per_player,
              const int32_t &state_delay_ms);
  bool step() override;

  ~SinglePlayerMode() {}
};