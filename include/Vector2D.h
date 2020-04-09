#ifndef DRAW_CONVEX_HULL_VECTOR2D_H
#define DRAW_CONVEX_HULL_VECTOR2D_H

template<Typename T>
class Vector2D {
  T x, y;

  Vector2D();
  Vector2D(T _x, T _y);

  double length() const;

  double arccos() const;

  bool operator==(const Vector2D<T> &rhs) const;

  bool operator!=(const Vector2D<T> &rhs) const;

  Vector2D<T> operator-(const Vector2D<T> &rhs) const;

  static bool compareVector2D(Vector2D<T> &a, Vector2D<T> &b);

  void print();

  virtual ~Vector2D();
};

#endif //DRAW_CONVEX_HULL_VECTOR2D_H
