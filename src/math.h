#ifndef MATH_H
#define MATH_H

/// A 3D vector class.
template <typename T> struct Vec3 {
  T x, y, z;

  /// Multiply by a constant.
  Vec3 operator*(float k) const { return Vec3(x * k, y * k, z * k); }

  /// Add two vectors.
  void operator+=(Vec3 other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }
};

/// A 2D vector class for `rotation`.
template <typename T> struct Vec2 {
  T x, y;
};

using Vec3f = Vec3<float>;
using Vec2f = Vec2<float>;

#endif
