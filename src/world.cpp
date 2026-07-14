#include "world.h"

void World::update(float dt) {
  for (uint32_t id = 0; id < entity_ids_count; ++id) {
    auto transformComponent = getComponent<TransformComponent>(id);
    auto speedComponent = getComponent<SpeedComponent>(id);

    if (transformComponent != nullptr && speedComponent != nullptr) {
      gravitySystem(dt, transformComponent, speedComponent);
    }
  }
}

EntityID World::addEntity() {
  if (!freed_entity_ids.empty()) {
    uint32_t id = freed_entity_ids.back();
    freed_entity_ids.pop_back();
    return id;
  }
  entity_components.emplace_back();
  return entity_ids_count++;
}

void World::removeEntity(EntityID id) {
  // removed will never be chosen by systems
  // as they do not have any components
  entity_components[id].clear();
  freed_entity_ids.push_back(id);
}
void World::addComponent(std::unique_ptr<Component> component, EntityID id) {
  entity_components[id].push_back(std::move(component));
}
template <typename T> T *World::getComponent(EntityID id) const {
  for (auto &component : entity_components[id]) {
    if (auto res = dynamic_cast<T *>(component.get())) {
      return res;
    }
  }
  return nullptr;
}

template <typename T> bool World::hasComponent(EntityID id) const {
  return getComponent<T>(id) != nullptr;
}
