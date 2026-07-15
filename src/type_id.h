#ifndef TYPE_ID_H
#define TYPE_ID_H

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
