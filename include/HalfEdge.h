#ifndef DRAW_CONVEX_HULL_HALFEDGE_H
#define DRAW_CONVEX_HULL_HALFEDGE_H

#include <Vertex.h>

class HalfEdge {
 public:
  using VertexRef   = Vertex*;
  using HalfEdgeRef = HalfEdge*;

  HalfEdge(VertexRef _origin, HalfEdgeRef _prev, HalfEdgeRef _next, HalfEdgeRef _twin);

  // for boundary edges
  HalfEdge(VertexRef _origin);
  ~HalfEdge();

  VertexRef getOrigin();
  HalfEdgeRef getTwin();
  HalfEdgeRef getNext();

  bool isVertexInHalfEdge(VertexRef vertexRef);

  void setPrev(HalfEdgeRef he);
  void setNext(HalfEdgeRef he);
  void setTwin(HalfEdgeRef he);

 private:
  // the 2D point origin of the half edge
  VertexRef origin;
  HalfEdgeRef twin, next, prev;

  static double det2D(const Vertex::VectorType p, const Vertex::VectorType q) {
    return p.x * q.y - q.x * p.y;
  }
};

#include <impl/HalfEdge.cpp>

#endif  // DRAW_CONVEX_HULL_HALFEDGE_H
