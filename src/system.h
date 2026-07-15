#ifndef SYSTEM_H
#define SYSTEM_H

#include <memory>

class World;

class System {
public:
  virtual ~System() = default;

  virtual void update(World &world, float dt) const = 0;
};

template <typename T> std::unique_ptr<System> make_system() {
  return std::make_unique<T>();
}

#endif
