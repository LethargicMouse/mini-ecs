#ifndef WORLD_H
#define WORLD_H

// no `cpp` file because templates

#include "common.h"
#include "pool.h"
#include "system.h"
#include "type_id.h"
#include <memory>
#include <vector>

using EntityID = ID;

template <typename... Ts> class View;

class World {
public:
  World() {}

  EntityID addEntity() {
    EntityID id = 0;
    if (!freed_ids.empty()) {
      id = freed_ids.back();
      freed_ids.pop_back();
    } else {
      id = id_pool_size++;
    }
    // store the ID in the ID pool
    addComponent(id, id);
    return id;
  }

  void removeEntity(EntityID id) {
    freed_ids.push_back(id);
    for (auto &pool : pools) {
      pool->remove(id);
    }
  }

  template <typename T> void addComponent(T component, EntityID id) {
    auto &pool = getPool<T>();
    pool.insert(id, component);
  }

  template <typename T> bool hasComponent(EntityID id) {
    auto &pool = getPool<T>();
    return pool.contains(id);
  }

  // not const because inner T can be modified with returned T*
  template <typename T> T *getComponent(EntityID id) {
    auto &pool = getPool<T>();
    return pool.get(id);
  }

  void update(float dt) {
    for (auto &system : systems) {
      system->update(*this, dt);
    }
  }

  // not const because inner vector<T> can be modified with returned vector<T>&
  template <typename T> std::vector<T> &getAll() {
    auto &pool = getPool<T>();
    return pool.getAll();
  }

  template <typename T> void addSystem() {
    systems.push_back(std::move(make_system<T>()));
  }

  template <typename... Ts> View<Ts...> view();

private:
  template <typename T> Pool<T> &getPool() {
    TypeID id = getTypeID<T>();
    if (id >= pools.size()) {
      pools.resize(id + 1);
    }
    if (pools[id].get() == nullptr) {
      pools[id] = std::make_unique<Pool<T>>();
    }
    return static_cast<Pool<T> &>(*pools[id]);
  }

  std::vector<EntityID> freed_ids;
  EntityID id_pool_size{};

  // `Component` class not needed - any type can be a component
  std::vector<std::unique_ptr<PoolAny>> pools;

  std::vector<std::unique_ptr<System>> systems;
};

// this whole class is a big template mess, God do I hate C++
template <typename... Ts> class View {
public:
  explicit View(World *world) : world(world) {}

  class Iterator {
  public:
    Iterator(World *world, size_t cursor) : world(world), cursor(cursor) {}

    Iterator &operator++() {
      ++cursor;
      gotoNextGood();
      return *this;
    }

    bool operator!=(const Iterator &other) const {
      return cursor != other.cursor;
    }

    std::tuple<Ts &...> operator*() const {
      return std::tie(*world->template getComponent<Ts>(
          world->getAll<EntityID>()[cursor])...);
    }

  private:
    bool isGood() {
      return (
          world->template hasComponent<Ts>(world->getAll<EntityID>()[cursor]) &&
          ...);
    }

    void gotoNextGood() {
      while (cursor < world->getAll<EntityID>().size() && !isGood()) {
        ++cursor;
      }
    }

    World *world;
    size_t cursor;
  };

  Iterator begin() const { return Iterator(world, 0); }

  Iterator end() const {
    return Iterator(world, world->getAll<EntityID>().size());
  }

private:
  World *world;
};

template <typename... Ts> inline View<Ts...> World::view() {
  return View<Ts...>(this);
}

#endif
