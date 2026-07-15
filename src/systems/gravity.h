#ifndef GRAVITY_H
#define GRAVITY_H

#include "../components/speed.h"
#include "../components/transform.h"
#include "../world.h"

inline constexpr float G = 10;

class GravitySystem : public System {
public:
  void update(World &world, float dt) const override {
    for (auto [transform, speed] : world.view<Transform, Speed>()) {
      transform.pos += speed.vec * dt;
      speed.vec.y += G * dt;
    }
  }
};

#endif
