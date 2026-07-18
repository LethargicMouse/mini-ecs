#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../math.h"
#include <string>

struct Transform {
  Vec3f pos;
  Vec2f rotation;
  Vec3f scale;

  static std::string type_name() { return "Transform"; }

  json to_json() const {
    json res;
    res["pos"] = pos.to_json();
    res["rotation"] = rotation.to_json();
    res["scale"] = scale.to_json();
    return res;
  }

  void from_json(const json &json) {
    pos.from_json(json["pos"]);
    rotation.from_json(json["rotation"]);
    scale.from_json(json["scale"]);
  }
};

#endif
