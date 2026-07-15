#ifndef HEALTH_H
#define HEALTH_H

#include <cstdint>

struct Health {
  Health(uint32_t max) : max(max), current(max) {}

  uint32_t max, current;
};

#endif
