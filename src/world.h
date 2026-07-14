#ifndef _WORLD_H_
#define _WORLD_H_

#include "component.h"
#include "system.h"
#include <cstdint>
#include <memory>
#include <vector>

using EntityID = uint32_t;

class World {
public:
  World() : entity_ids_count(0), freed_entity_ids(), entity_components() {}

  void update(float dt) {
    for (uint32_t id = 0; id < entity_ids_count; ++id) {
      auto transformComponent = getComponent<TransformComponent>(id);
      auto speedComponent = getComponent<SpeedComponent>(id);

      if (transformComponent != nullptr && speedComponent != nullptr) {
        gravitySystem(dt, transformComponent, speedComponent);
      }
    }
  }

  EntityID addEntity() {
    if (!freed_entity_ids.empty()) {
      uint32_t id = freed_entity_ids.back();
      freed_entity_ids.pop_back();
      return id;
    }
    entity_components.emplace_back();
    return entity_ids_count++;
  }

  void removeEntity(EntityID id) {
    // removed will never be chosen by systems
    // as they do not have any components
    entity_components[id].clear();
    freed_entity_ids.push_back(id);
  }

  void addComponent(std::unique_ptr<Component> component, EntityID id) {
    entity_components[id].push_back(std::move(component));
  }

  template <typename T> T *getComponent(EntityID id) const {
    for (auto &component : entity_components[id]) {
      if (auto res = dynamic_cast<T *>(component.get())) {
        return res;
      }
    }
    return nullptr;
  }

private:
  template <typename T> bool hasComponent(EntityID id) const {
    return getComponent<T>(id) != nullptr;
  }

  uint32_t entity_ids_count;
  std::vector<uint32_t> freed_entity_ids;
  std::vector<std::vector<std::unique_ptr<Component>>> entity_components;
};

#endif
