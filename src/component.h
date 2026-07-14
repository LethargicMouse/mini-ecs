#ifndef _COMPONENT_H_
#define _COMPONENT_H_

#include "math.h"
#include <cstdint>

class Component {
public:
  virtual ~Component() = default;
};

class TransformComponent : public Component {
public:
  TransformComponent() : pos(0, 0, 0), rotation(0, 0), scale(1, 1, 1) {}

  Vec3f pos;
  Vec2f rotation;
  Vec3f scale;
};

class SpeedComponent : public Component {
public:
  SpeedComponent() : speed(0, 0, 0) {}

  Vec3f speed;
};

class HealthComponent : public Component {
public:
  HealthComponent(uint32_t max_health)
      : max_health(max_health), current_health(max_health) {}

  uint32_t max_health;
  uint32_t current_health;
};

#endif
