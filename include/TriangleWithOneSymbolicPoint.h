#ifndef DRAW_CONVEX_HULL_TRIANGLEWITHONESYMBOLICPOINT_H
#define DRAW_CONVEX_HULL_TRIANGLEWITHONESYMBOLICPOINT_H

#include <Triangle.h>

class TriangleWithOneSymbolicPoint : public Triangle {
 public:
  Vertex::Symbol symbol;

  TriangleWithOneSymbolicPoint();
  TriangleWithOneSymbolicPoint(Vertex::Symbol symbol, VertexRef a, VertexRef b);

  /** check if point is inside triangle. [includes boundary]
   * Since the triangle has one symbolic vertex, containsPoint
   * really is checking if query point is to the left or right
   * of HalfEdge ab.
   * */
  bool containsPoint(VertexRef p) const override;
  ChildrenType splitFace(VertexRef p) override;

  HalfEdgeRef halfEdgeContainsPoint(VertexRef p) override;

  Orientation getOrientation() override;

  ~TriangleWithOneSymbolicPoint() override = default;

 private:
  void setOrientation() override;
  ChildrenType splitFace(VertexRef p);
};

#include <impl/TriangleWithOneSymbolicPoint.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLEWITHONESYMBOLICPOINT_H
