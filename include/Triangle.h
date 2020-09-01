#ifndef DRAW_CONVEX_HULL_TRIANGLE_H
#define DRAW_CONVEX_HULL_TRIANGLE_H

#include <include/HalfEdge.h>
#include <include/Vertex.h>

class Triangle {
 public:
  using VertexRef   = Vector2D<double>*;
  using HalfEdgeRef = HalfEdge*;
  using TriangleRef = Triangle*;

  static const uint8_t NUM_VERTICES_PER_FACE = 3;

  enum Orientation { positive = +1, negative = -1, unset = 0 };

  HalfEdgeRef he;
  VertexRef a, b, c;
  using ChildrenType = std::array<TriangleRef, NUM_VERTICES_PER_FACE>;

  explicit Triangle(HalfEdgeRef he);
  Triangle(VertexRef a, VertexRef b, VertexRef c);

  virtual Orientation getOrientation();

  /** check if point is inside triangle. [includes boundary] */
  virtual bool containsPoint(VertexRef p) const;
  virtual ChildrenType splitFace(VertexRef p);

  virtual HalfEdgeRef halfEdgeContainsPoint(VertexRef p);

  virtual ~Triangle();


 protected:
  Orientation orientation;
  virtual void setOrientation();
};

#include <impl/Triangle.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLE_H
