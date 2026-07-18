#ifndef HEALTH_H
#define HEALTH_H

#include "../common.h"

struct Health {
  Health() : max(0), current(0) {}
  Health(uint32_t max) : max(max), current(max) {}

  uint32_t max, current;

  /// Required for serialization.
  static std::string type_name() { return "Health"; }

  /// Serialization.
  json to_json() const {
    json res;
    res["max"] = max;
    res["current"] = current;
    return res;
  }

  /// Deserialization.
  void from_json(const json &json) {
    max = json["max"];
    current = json["current"];
  }
};

#endif
