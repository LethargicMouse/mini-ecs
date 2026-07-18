#ifndef SPEED_H
#define SPEED_H

#include "../math.h"
#include <string>

struct Speed {
  Vec3f vec;

  /// Required for serialization.
  static std::string type_name() { return "Speed"; }

  /// serialization.
  json to_json() const { return vec.to_json(); }

  /// Deserialization.
  void from_json(const json &json) { vec.from_json(json); }
};

#endif
