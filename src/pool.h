#ifndef POOL_H
#define POOL_H

#include "common.h"
#include <cassert>
#include <vector>

class PoolAny {
public:
  virtual ~PoolAny() = default;

  /// This method is defined in the base class so we can try removing Components
  /// of an Entity being removed without knowing the exact types.
  virtual void remove(ID id) = 0;

  /// Check if the pool is empty.
  virtual bool is_empty() const = 0;

  /// Get the `type_name` of the elements.
  virtual std::string type_name() const = 0;

  /// Serialize the pool to json assuming the values are serializable.
  virtual json to_json() const = 0;

  /// Deserialize the pool from json assuming the values are deserializable.
  virtual void from_json(const json &json) = 0;
};

/// A generic data structure with fast insert/remove/get/contains queries
/// The structure becomes inefficient in memory if the `ID`s are too large.
/// For us it is not the case.
///
/// @tparam  T  The type being stored.
template <typename T> class Pool : public PoolAny {
public:
  Pool() {}

  /// @brief  Insert a value in a cell with a given ID.
  ///
  /// Time complexity: O(1).
  ///
  /// @param  id  The ID of the cell to insert in.
  /// @param  value  The value to insert.
  void insert(ID id, T value) {
    if (id >= id_map.size()) {
      id_map.resize(id + 1, SIZE_MAX);
    }
    assert(id_map[id] == SIZE_MAX);
    id_map[id] = data.size();
    ids.push_back(id);
    data.push_back(value);
  }

  /// @brief  Remove the value from the cell with a given ID.
  ///
  /// Time complexity: O(1).
  ///
  /// @param  id  The ID of the cell to remove from.
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
    id_map[id] = SIZE_MAX;
  }

  /// @brief  Check if the cell with a given ID contains a value.
  ///
  /// Time complexity: O(1)
  ///
  /// @param  id  The ID of a cell to check.
  bool contains(ID id) const {
    return id < id_map.size() && id_map[id] != SIZE_MAX;
  }

  /// @brief  Get a pointer to the value in the cell with a given ID.
  ///
  /// Time complexity: O(1).
  /// Not marked as const because inner `T can` be modified with returned `T*`.
  ///
  /// @param  id  The ID of a cell to get from.
  /// @return  The pointer to the value or `nullptr`
  ///          if there is no value in the cell.
  T *get(ID id) {
    if (!contains(id)) {
      return nullptr;
    }
    return &data[id_map[id]];
  }

  /// @brief  Get all the values stored.
  ///
  /// Time complexity: O(1).
  /// Not marked as const because inner `vector<T>` can
  /// be modified with returned `vector<T>&`.
  ///
  /// @return  A reference to the `vector` with all the values stored.
  std::vector<T> &getAll() { return data; }

  /// @brief  Check if the Pool is empty.
  ///
  /// Time complexity: O(1)
  bool is_empty() const override { return data.empty(); }

  /// Get the elements' `type_name`.
  std::string type_name() const override { return T::type_name(); }

  /// Serialization.
  json to_json() const override {
    json res = json::array();
    for (size_t i = 0; i < data.size(); ++i) {
      res.push_back({
          {"id", ids[i]},
          {"value", data[i].to_json()},
      });
    }
    return res;
  }

  /// Deserialization.
  void from_json(const json &json) override {
    data.clear();
    ids.clear();
    id_map.clear();

    for (const auto &pair : json) {
      ID id = pair["id"];
      T value;
      value.from_json(pair["value"]);
      insert(id, value);
    }
  }

private:
  /// The vector with all the values.
  std::vector<T> data;
  /// The map from `ID`s to indices of `data`.
  /// Stores SIZE_MAX for free `ID`s.
  std::vector<size_t> id_map;
  /// The `ID`s of the values.
  std::vector<ID> ids;
};

#endif
