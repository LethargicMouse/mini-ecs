#ifndef SPEED_H
#define SPEED_H

#include "../math.h"
#include <string>

struct Speed {
  Vec3f vec;

  static std::string type_name() { return "Speed"; }

  json to_json() const { return vec.to_json(); }

  void from_json(const json &json) { vec.from_json(json); }
};

#endif
