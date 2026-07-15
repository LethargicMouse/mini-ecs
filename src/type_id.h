#ifndef TYPE_ID_H
#define TYPE_ID_H

// This should only be included in `world.h`
// so that we have only one static `next` var

#include "common.h"

using TypeID = ID;

inline TypeID nextTypeID() {
  static TypeID next = 0;
  return next++;
}

template <typename T> TypeID getTypeID() {
  static const TypeID id = nextTypeID();
  return id;
}

#endif
