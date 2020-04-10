#ifndef DRAW_CONVEX_HULL_VECTOR2D_H
#define DRAW_CONVEX_HULL_VECTOR2D_H

template<typename T>
class Vector2D {
 public:
  T x, y;

  Vector2D();
  Vector2D(T _x, T _y);

  double length() const;

  double arccos() const;

  bool operator==(const Vector2D &rhs) const;

  bool operator!=(const Vector2D &rhs) const;

  Vector2D operator-(const Vector2D &rhs) const;

  Vector2D operator+(const Vector2D &rhs) const;

  static bool compareVector2D(Vector2D &a, Vector2D &b);

  void print();

  virtual ~Vector2D();
};


// include implementation file here so there is no need
// explicitly instantiate template for specific types.
// Otherwise one gets an "undefined reference to"
// linking error.
#include "include/impl/Vector2D.cpp"

#endif //DRAW_CONVEX_HULL_VECTOR2D_H
