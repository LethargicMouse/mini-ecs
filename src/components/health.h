#ifndef HEALTH_H
#define HEALTH_H

#include "../common.h"

struct Health {
  Health() : max(0), current(0) {}
  Health(uint32_t max) : max(max), current(max) {}

  uint32_t max, current;

  static std::string type_name() { return "Health"; }

  json to_json() const {
    json res;
    res["max"] = max;
    res["current"] = current;
    return res;
  }

  void from_json(const json &json) {
    max = json["max"];
    current = json["current"];
  }
};

#endif
