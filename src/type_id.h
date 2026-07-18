#ifndef TYPE_ID_H
#define TYPE_ID_H

// This should only be included in `world.h`
// so that we have only one static `next` var

#include "common.h"

using TypeID = ID;

/// Return the value of the static counter and increase it.
inline TypeID nextTypeID() {
  static TypeID next = 0;
  return next++;
}

/// Gets a unique value of the counter once for every type.
/// Only stable if this header is included exactly once.
template <typename T> TypeID getTypeID() {
  static const TypeID id = nextTypeID();
  return id;
}

#endif
