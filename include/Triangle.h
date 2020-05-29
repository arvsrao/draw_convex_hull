#ifndef DRAW_CONVEX_HULL_TRIANGLE_H
#define DRAW_CONVEX_HULL_TRIANGLE_H

#include <include/Vector2D.h>

template <typename T>
class Triangle {
 public:
  using PointType = Vector2D<T>;
  PointType a, b, c;

  Triangle(PointType _a, PointType _b, PointType _c);

  /** check if point is inside triangle. [includes boundary] */
  bool containsPoint(PointType& point);

  virtual ~Triangle();
};

#include <impl/Triangle.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLE_H
