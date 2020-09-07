#ifndef DRAW_CONVEX_HULL_HALFEDGE_H
#define DRAW_CONVEX_HULL_HALFEDGE_H

#include <Vertex.h>

class HalfEdge {
 public:
  using VertexRef   = Vector2D<double>*;
  using HalfEdgeRef = HalfEdge*;

  HalfEdge(VertexRef origin, HalfEdgeRef prev, HalfEdgeRef next, HalfEdgeRef twin);

  // for boundary edges
  explicit HalfEdge(Vertex::Symbol symbol);
  explicit HalfEdge(VertexRef origin);
  ~HalfEdge();

  VertexRef getOrigin() const;
  HalfEdgeRef getTwin() const;
  HalfEdgeRef getNext() const;
  HalfEdgeRef getPrev() const;
  Vertex::Symbol getSymbol() const;
  bool hasSymbol() const;

  bool isVertexInHalfEdge(VertexRef vertexRef);

  void setPrev(HalfEdgeRef he);
  void setNext(HalfEdgeRef he);
  void setTwin(HalfEdgeRef he);

 private:
  // the 2D point origin of the half edge
  Vertex::Symbol symbol;
  VertexRef origin;
  HalfEdgeRef twin, next, prev;
};

#include <impl/HalfEdge.cpp>

#endif  // DRAW_CONVEX_HULL_HALFEDGE_H
