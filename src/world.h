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

/// A class that runs the ECS and stores everything.
class World {
public:
  /// @brief creates a `World` with no Entities and no Systems
  ///
  /// Time complexity: O(1)
  World() {}

  /// @brief  Registers a new Entity.
  ///
  /// Time complexity: O(1)
  ///
  /// @return  The ID of the new entity.
  ///
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

  /// @brief  Removes an Entity and clears its Components.
  ///
  /// Time complexity: O(C) where C is the number of Components ever registered.
  ///
  /// I could add a `Pool<vector<TypeID>>` that would store every type of
  /// Component an Entity has, and update it for O(1) on each `addComponent`,
  /// then this operation will become O(c)
  /// where c is the number of Components the Entity has,
  /// but I think the time loss from all the allocations needed is bigger
  /// than the gain from the better complexity.
  ///
  /// @param  id  The ID of the Entity that was returned by `addEntity`.
  void removeEntity(EntityID id) {
    freed_ids.push_back(id);
    for (auto &pool : pools) {
      pool->remove(id);
    }
  }

  /// @brief  Add a Component to an Entity.
  ///
  /// Time complexity: O(1)
  ///
  /// If the Entity already has a Component of this type,
  /// it will be replaced by the new one.
  ///
  /// @tparam  T  The Component type.
  /// @param  component  The Component.
  /// @param  id  The ID of the Entity.
  template <typename T> void addComponent(T component, EntityID id) {
    auto &pool = getPool<T>();
    pool.insert(id, component);
  }

  /// @brief  Check if an Entity has a Component of given type.
  ///
  /// Time complexity: O(1)
  ///
  /// @tparam  T  The Component type.
  /// @param  id  The ID of the Entity.
  /// @return  true if the Entity has the Component, false if not.
  template <typename T> bool hasComponent(EntityID id) {
    auto &pool = getPool<T>();
    return pool.contains(id);
  }

  /// @brief  Get the pointer to an Entity's Component of type `T`.
  ///
  /// Time complexity: O(1).
  ///
  /// Not marked as const because inner `T` can
  /// be modified with returned `T*`.
  ///
  /// @tparam  T  The Component type.
  /// @param  id  The ID of the Entity.
  /// @return  The pointer to the Component or `nullptr`
  ///          if the Entity does not have a Component of this type.
  template <typename T> T *getComponent(EntityID id) {
    auto &pool = getPool<T>();
    return pool.get(id);
  }

  /// @brief  update the `World` state by running all the attached Systems.
  ///
  /// @param  dt  the time in seconds from the last tick.
  void update(float dt) {
    for (auto &system : systems) {
      system->update(*this, dt);
    }
  }

  /// @brief  Get all the Components of a given type.
  ///
  /// Time complexity: O(1).
  ///
  /// Not marked as const because inner `vector<T>` can
  /// be modified with returned `vector<T>&`.
  ///
  /// @tparam  T  The Components type.
  /// @return  The reference to the `vector` of all the Components of type `T`.
  template <typename T> std::vector<T> &getAll() {
    auto &pool = getPool<T>();
    return pool.getAll();
  }

  /// @brief  Register a new System.
  ///
  /// Time complexity: O(1).
  ///
  /// @tparam  T  The subclass of the `System` class.
  template <typename T> void addSystem() {
    systems.push_back(std::move(make_system<T>()));
  }

  /// @brief  Query all the Entities with Components of given types.
  ///
  /// Time complexity: O(1).
  /// Worst case time complexity of the whole traverse: O(N * T)
  /// where N is the number of currently registered Entities,
  /// and T is the number of Components queried.
  ///
  /// @tparam  Ts  The types of the Components.
  /// @return  A range to iterate over the Components.
  /// @note  The body is defined below the `View` class
  ///        for mutual reference reasons.
  template <typename... Ts> View<Ts...> view();

private:
  /// @brief  Get the `Pool` of Components of a given type.
  ///
  /// Time complexity: O(1)
  ///
  /// @tparam  T  The type of Components.
  /// @return  A reference to the `Pool` of the Components.
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

  /// The stack of freed `EntityID`s for reuse.
  std::vector<EntityID> freed_ids;
  EntityID id_pool_size{};

  /// The vector of `Pool`s for each type ever registered as a Component.
  /// `Component` class not needed - any type can be a component
  std::vector<std::unique_ptr<PoolAny>> pools;

  /// The vector of all registered Systems
  std::vector<std::unique_ptr<System>> systems;
};

// This whole class is a big template mess, God do I hate C++
/// @brief A class that looks for entities with
/// a given set of Components and iterates over them.
/// @tparam  Ts  The types of Components.
template <typename... Ts> class View {
public:
  /// @brief  Creates a range to iterate over the queried Components.
  ///
  /// Time complexity: O(1).
  ///
  /// @param  world  A pointer to `World`.
  explicit View(World *world) : world(world) {}

  /// An iterator over queried Components.
  class Iterator {
  public:
    /// @brief  Creates an iterator.
    ///
    /// Time complexity: O(1).
    ///
    /// @param  world  A pointer to `World`.
    /// @param  cursor  An index of the Entity to start from.
    Iterator(World *world, size_t cursor) : world(world), cursor(cursor) {}

    /// @brief  Find next fitting Entity after the current.
    ///
    /// Time complexity: O(B * T)
    /// where B is the number of Entities
    /// between the current and the next fitting,
    /// and T is the number of Components queried.
    Iterator &operator++() {
      ++cursor;
      gotoNextGood();
      return *this;
    }

    bool operator!=(const Iterator &other) const {
      return cursor != other.cursor;
    }

    /// @brief  Get the queried Components of the current Entity.
    ///
    /// Time complexity: O(T)
    /// where T is the number of Components queried.
    std::tuple<Ts &...> operator*() const {
      return std::tie(*world->template getComponent<Ts>(
          world->getAll<EntityID>()[cursor])...);
    }

  private:
    /// @brief  Check if the current Entity fits.
    ///
    /// Time complexity: O(T)
    /// where T is the number of Components queried.
    bool isGood() {
      return (
          world->template hasComponent<Ts>(world->getAll<EntityID>()[cursor]) &&
          ...);
    }

    /// @brief  Find next fitting Entity starting from the current.
    ///
    /// Time complexity: O(B * T)
    /// where B is the number of Entities
    /// between the current and the next fitting,
    /// and T is the number of Components queried.
    void gotoNextGood() {
      while (cursor < world->getAll<EntityID>().size() && !isGood()) {
        ++cursor;
      }
    }

    /// A pointer to `World`.
    World *world;
    /// An index of the current Entity.
    size_t cursor;
  };

  Iterator begin() const { return Iterator(world, 0); }

  Iterator end() const {
    return Iterator(world, world->getAll<EntityID>().size());
  }

private:
  /// A pointer to `World`.
  World *world;
};

template <typename... Ts> inline View<Ts...> World::view() {
  return View<Ts...>(this);
}

#endif
