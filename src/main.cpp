#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>

class Entity {
public:
  Entity(uint32_t id) : id(id) {}

  uint32_t id;
};

class Component {
public:
  virtual ~Component() = default;
};

template <typename T> class Vec2 {
public:
  Vec2(T x, T y) : x(x), y(y) {}

  T x, y;
};

template <typename T> class Vec3 {
public:
  Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

  Vec3 operator*(float k) const { return Vec3(x * k, y * k, z * k); }
  void operator+=(Vec3 other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }

  T x, y, z;
};

using Vec2f = Vec2<float>;
using Vec3f = Vec3<float>;

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

void gravitySystem(float dt, TransformComponent *transformComponent,
                   SpeedComponent *speedComponent) {
  speedComponent->speed.y += 10 * dt;
  transformComponent->pos += speedComponent->speed * dt;
}

class World {
public:
  World() : entity_ids_count(0), freed_entity_ids(), entity_components() {}

  void update(float dt) {
    for (uint32_t id = 0; id < entity_ids_count; ++id) {
      auto transformComponent = getComponent<TransformComponent>(Entity(id));
      auto speedComponent = getComponent<SpeedComponent>(Entity(id));

      if (transformComponent != nullptr && speedComponent != nullptr) {
        gravitySystem(dt, transformComponent, speedComponent);
      }
    }
  }

  Entity addEntity() {
    if (!freed_entity_ids.empty()) {
      uint32_t id = freed_entity_ids.back();
      freed_entity_ids.pop_back();
      return Entity(id);
    }
    entity_components.emplace_back();
    return Entity(entity_ids_count++);
  }

  void removeEntity(Entity entity) {
    // removed will never be chosen by systems
    // as they do not have any components
    entity_components[entity.id].clear();
    freed_entity_ids.push_back(entity.id);
  }

  void addComponent(std::unique_ptr<Component> component, Entity entity) {
    entity_components[entity.id].push_back(std::move(component));
  }

  template <typename T> T *getComponent(Entity entity) const {
    for (auto &component : entity_components[entity.id]) {
      if (auto res = dynamic_cast<T *>(component.get())) {
        return res;
      }
    }
    return nullptr;
  }

private:
  template <typename T> bool hasComponent(Entity entity) const {
    return getComponent<T>(entity) != nullptr;
  }

  uint32_t entity_ids_count;
  std::vector<uint32_t> freed_entity_ids;
  std::vector<std::vector<std::unique_ptr<Component>>> entity_components;
};

template <typename T> std::unique_ptr<Component> make_component() {
  return std::make_unique<T>();
}

int main() {
  World world;
  Entity player = world.addEntity();
  Entity enemy = world.addEntity();
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
