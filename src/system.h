#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>

class World;

class System {
public:
  virtual ~System() = default;

  /// The System has unlimited access to the `World`,
  /// it can query any sets of Components, add and remove Entities,
  /// add and remove Components.
  /// The behaviour is unstable if the System adds/removes Entities/Components
  /// while iterating over `View`.
  /// The System must not call `World::update`.
  virtual void update(World &world, float dt) const = 0;
};

/// Allocates a system of a given type into `unique_ptr`.
template <typename T> std::unique_ptr<System> make_system() {
  return std::make_unique<T>();
}

#endif
