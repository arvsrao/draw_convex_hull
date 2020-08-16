#ifndef DRAW_CONVEX_HULL_TRIANGLE_H
#define DRAW_CONVEX_HULL_TRIANGLE_H

#include <include/HalfEdge.h>
#include <include/Vertex.h>

class Triangle {
 public:
  using VertexRef   = Vertex*;
  using HalfEdgeRef = HalfEdge*;

  static const uint8_t NUM_VERTICES_PER_FACE;

  HalfEdgeRef he;
  VertexRef a, b, c;

  Triangle(HalfEdgeRef _he);
  Triangle(VertexRef _a, VertexRef _b, VertexRef _c);

  /** check if point is inside triangle. [includes boundary] */
  virtual bool containsPoint(const VertexRef p) const;

  virtual HalfEdgeRef halfEdgeContainsPoint(VertexRef p);

  virtual ~Triangle();
};

#include <impl/Triangle.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLE_H
