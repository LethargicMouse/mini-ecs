#include "world.h"
#include <iostream>
#include <memory>

template <typename T> std::unique_ptr<Component> make_component() {
  return std::make_unique<T>();
}

int main() {
  World world;
  EntityID player = world.addEntity();
  EntityID enemy = world.addEntity();
  auto player_transform = make_component<TransformComponent>();
  auto enemy_transform = make_component<TransformComponent>();
  auto player_speed = make_component<SpeedComponent>();
  auto enemy_speed = make_component<SpeedComponent>();
  world.addComponent(std::move(player_transform), player);
  world.addComponent(std::move(enemy_transform), enemy);
  world.addComponent(std::move(player_speed), player);
  world.addComponent(std::move(enemy_speed), enemy);
  world.update(1);
  world.update(1);
  world.update(1);
  auto transform = world.getComponent<TransformComponent>(player);
  std::cout << "player pos: " << transform->pos.x << ' ' << transform->pos.y
            << ' ' << transform->pos.z << '\n';
}
