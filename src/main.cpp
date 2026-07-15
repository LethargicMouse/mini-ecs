// This is my second attempt for this task.
// To see the first attempt, checkout the last commit before `reset`.

#include "components/health.h"
#include "components/speed.h"
#include "components/transform.h"
#include "systems/gravity.h"
#include "world.h"
#include <iostream>

inline constexpr uint32_t ENEMY_MAX_HEALTH = 100;

int main() {
  World world;

  EntityID player = world.addEntity();
  EntityID enemy = world.addEntity();

  world.addComponent(Transform(), player);
  world.addComponent(Speed(), player);

  world.addComponent(Transform(), enemy);
  world.addComponent(Health(ENEMY_MAX_HEALTH), enemy);

  world.addSystem<GravitySystem>();

  for (int _ = 0; _ < 3; ++_) {
    world.update(1);
  }

  Transform *player_transform = world.getComponent<Transform>(player);
  assert(player_transform != nullptr);
  std::cout << player_transform->pos.x << ' ' << player_transform->pos.y << ' '
            << player_transform->pos.z << '\n';
}
