#ifndef _MATH_H_
#define _MATH_H_

template <typename T> class Vec2 {
public:
  Vec2(T x, T y) : x(x), y(y) {}

  T x, y;
};

template <typename T> class Vec3 {
public:
  Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

  Vec3 operator*(float k) const { return Vec3(x * k, y * k, z * k); }

  void operator+=(Vec3<T> other) {
    x += other.x;
    y += other.y;
    z += other.z;
  }

  T x, y, z;
};

using Vec2f = Vec2<float>;
using Vec3f = Vec3<float>;

#endif
