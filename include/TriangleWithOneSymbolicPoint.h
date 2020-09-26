#ifndef DRAW_CONVEX_HULL_TRIANGLEWITHONESYMBOLICPOINT_H
#define DRAW_CONVEX_HULL_TRIANGLEWITHONESYMBOLICPOINT_H

#include <Triangle.h>

class TriangleWithOneSymbolicPoint : public Triangle {
 public:
  TriangleWithOneSymbolicPoint();
  TriangleWithOneSymbolicPoint(HalfEdge::Symbol symbol, VertexRef a, VertexRef b);

  /** check if point is inside triangle. [includes boundary]
   * Since the triangle has one symbolic vertex, containsPoint
   * really is checking if query point is to the left or right
   * of HalfEdge ab.
   * */
  bool containsPoint(VertexRef p) const override;
  NewEdgeRefsContainerType splitFace(VertexRef p) override;
  NewEdgeRefsContainerType splitEdge(HalfEdgeRef ref, VertexRef q, VertexRef p) override;

  Orientation getOrientation() override;
  void splitEdgeHelper(HalfEdgeRef ref, VertexRef q, VertexRef p) override;

  ~TriangleWithOneSymbolicPoint() override = default;

 private:
  HalfEdge::Symbol symbol;
  void setOrientation() override;
};

#include <impl/TriangleWithOneSymbolicPoint.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLEWITHONESYMBOLICPOINT_H
