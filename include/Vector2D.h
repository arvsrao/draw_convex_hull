#ifndef DRAW_CONVEX_HULL_VECTOR2D_H
#define DRAW_CONVEX_HULL_VECTOR2D_H

template<typename T>
class Vector2D {
 public:
  T x, y;

  Vector2D();
  Vector2D(const Vector2D<int> &other);
  //Vector2D(const Vector2D<int> &&other);

  Vector2D(T _x, T _y);

  double length() const;

  double arccos() const;

  T dot(const Vector2D &other) const;

  bool operator==(const Vector2D &rhs) const;

  bool operator!=(const Vector2D &rhs) const;

  Vector2D operator-(const Vector2D &rhs) const;

  Vector2D operator+(const Vector2D &rhs) const;

  static bool compareVector2D(const Vector2D &a, const Vector2D &b);

  void print();

  virtual ~Vector2D();
};


// include implementation file here so there is no need explicitly instantiate template for specific types.
// Otherwise one gets an "undefined reference to" linking error.
// See https://bytefreaks.net/programming-2/c/c-undefined-reference-to-templated-class-function
#include "include/impl/Vector2D.cpp"

#endif //DRAW_CONVEX_HULL_VECTOR2D_H
