#ifndef DRAW_CONVEX_HULL_VECTOR2D_H
#define DRAW_CONVEX_HULL_VECTOR2D_H

template<Typename T>
class Vector2D {
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

#endif //DRAW_CONVEX_HULL_VECTOR2D_H
