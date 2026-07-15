#ifndef MATH_H
#define MATH_H

template <typename T> struct Vec3 {
  T x, y, z;

  Vec3 operator*(float k) const { return Vec3(x * k, y * k, z * k); }

  void operator+=(Vec3 other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }
};

template <typename T> struct Vec2 {
  T x, y;
};

using Vec3f = Vec3<float>;
using Vec2f = Vec2<float>;

#endif
