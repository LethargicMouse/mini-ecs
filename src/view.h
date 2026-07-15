#ifndef VIEW_H
#define VIEW_H

// this whole class is a big messy template magic, God do I hate C++

#include "world.h"

template <typename... Ts> class View {
public:
  explicit View(World *world) : world(world) {}

  class Iterator {
  public:
    Iterator(World *world, size_t cursor) : world(world), cursor(cursor) {}

    Iterator &operator++() {
      ++cursor;
      gotoNextGood();
    }

    bool operator!=(const Iterator &other) const {
      return cursor != other.cursor;
    }

    std::tuple<Ts &...> operator*() const {
      return std::tie(
          world->getComponent<Ts...>(world->getAll<EntityID>()[cursor]));
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

#endif
