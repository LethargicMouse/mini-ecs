#ifndef _WORLD_H_
#define _WORLD_H_

#include "system.h"
#include <cstdint>
#include <memory>
#include <vector>

using EntityID = uint32_t;

class World {
public:
  World() : entity_ids_count(0), freed_entity_ids(), entity_components() {}

  void update(float dt);

  EntityID addEntity();

  void removeEntity(EntityID id);

  void addComponent(std::unique_ptr<Component> component, EntityID id);

  template <typename T> T *getComponent(EntityID id) const;

  template <typename T> bool hasComponent(EntityID id) const;

private:
  uint32_t entity_ids_count;
  std::vector<uint32_t> freed_entity_ids;
  std::vector<std::vector<std::unique_ptr<Component>>> entity_components;
};

#endif
