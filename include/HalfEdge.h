#ifndef DRAW_CONVEX_HULL_HALFEDGE_H
#define DRAW_CONVEX_HULL_HALFEDGE_H

#include <Vector2D.h>

template <typename T>
class HalfEdge {
 public:
  using VertexRef   = Vector2D<T>*;
  using HalfEdgeRef = HalfEdge<T>*;

  HalfEdge(VertexRef _origin, HalfEdgeRef _prev, HalfEdgeRef _next, HalfEdgeRef _twin);

  // for boundary edges
  HalfEdge(VertexRef _origin);
  ~HalfEdge();

  VertexRef getOrigin();
  HalfEdgeRef getTwin();
  HalfEdgeRef getNext();

  void setPrev(HalfEdgeRef he);
  void setNext(HalfEdgeRef he);
  void setTwin(HalfEdgeRef he);

 private:
  // the 2D point origin of the half edge
  VertexRef origin;
  HalfEdgeRef twin, next, prev;
};

#include <impl/HalfEdge.cpp>

#endif  // DRAW_CONVEX_HULL_HALFEDGE_H
