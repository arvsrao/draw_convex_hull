#ifndef DRAW_CONVEX_HULL_TRIANGLEWITHTWOSYMBOLICPOINTS_H
#define DRAW_CONVEX_HULL_TRIANGLEWITHTWOSYMBOLICPOINTS_H

#include <Triangle.h>

class TriangleWithTwoSymbolicPoints : public Triangle {
 public:
  TriangleWithTwoSymbolicPoints(VertexRef p);

  /** True when the given point is below (less than) the not symbolic point
   *  p0 in triangle { Symbol::Left, p0, Symbol::Right }.
   */
  bool containsPoint(VertexRef p) const override;
  Orientation getOrientation() override;

  ~TriangleWithTwoSymbolicPoints() override = default;

 private:
  void setOrientation() override;
};

#include <impl/TriangleWithTwoSymbolicPoints.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLEWITHTWOSYMBOLICPOINTS_H
