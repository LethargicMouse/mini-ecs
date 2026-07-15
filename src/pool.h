#ifndef POOL_H
#define POOL_H

// no `.cpp` file because template

#include "common.h"
#include <cassert>
#include <vector>

class PoolAny {
public:
  virtual ~PoolAny() = default;

  virtual void remove(ID id) = 0;
};

template <typename T> class Pool : public PoolAny {
public:
  Pool() {}

  void insert(ID id, T value) {
    if (id >= id_map.size()) {
      id_map.resize(id + 1, SIZE_MAX);
    }
    assert(id_map[id] == SIZE_MAX);
    id_map[id] = data.size();
    ids.push_back(id);
    data.push_back(value);
  }

  void remove(ID id) override {
    if (!contains(id)) {
      return;
    }
    size_t index = id_map[id];
    size_t back_index = data.size() - 1;
    ID back_id = ids[back_index];
    std::swap(data[index], data[back_index]);
    std::swap(ids[index], ids[back_index]);
    std::swap(id_map[id], id_map[back_id]);
    data.pop_back();
    ids.pop_back();
  }

  bool contains(ID id) const {
    return id < id_map.size() && id_map[id] != SIZE_MAX;
  }

  // not const because inner T can be modified with returned T*
  T *get(ID id) {
    if (!contains(id)) {
      return nullptr;
    }
    return &data[id_map[id]];
  }

  // not const because inner vector<T> can be modified with returned vector<T>&
  std::vector<T> &getAll() { return data; }

private:
  std::vector<T> data;
  std::vector<size_t> id_map;
  std::vector<ID> ids;
};

#endif
