#ifndef DRAW_CONVEX_HULL_HALFEDGE_H
#define DRAW_CONVEX_HULL_HALFEDGE_H

#include <Vector2D.h>

// forward declaring Triangle is enough to reference it.
class Triangle;

class HalfEdge {
 public:
  using RingType    = double;
  using VertexRef   = Vector2D<RingType>*;
  using HalfEdgeRef = HalfEdge*;
  using TriangleRef = Triangle*;

  // None : the vertex is not a symbolic point
  // Left : the vertex is symbolic and to the left && above all other vertices
  // Right : the vertex is symbolic and to the right && below all other vertices
  enum Symbol { Left = -2, None = +1, Right = -1 };

  HalfEdge(VertexRef origin, HalfEdgeRef prev, HalfEdgeRef next, HalfEdgeRef twin);

  // for boundary edges
  explicit HalfEdge(Symbol symbol);
  explicit HalfEdge(VertexRef origin);
  ~HalfEdge();

  VertexRef getOrigin() const;
  HalfEdgeRef getTwin() const;
  HalfEdgeRef getNext() const;
  HalfEdgeRef getPrev() const;
  Symbol getSymbol() const;
  TriangleRef getTriangleRef() const;

  /** return the symbol type of either point */
  bool hasSymbol() const;

  bool isVertexInHalfEdge(VertexRef vertexRef);

  void setPrev(HalfEdgeRef he);
  void setNext(HalfEdgeRef he);
  void setTwin(HalfEdgeRef he);
  void setSymbol(Symbol s);
  void setTriangleRef(TriangleRef ref);

 private:
  // the 2D point origin of the half edge
  Symbol symbol;
  VertexRef origin;
  HalfEdgeRef twin, next, prev;
  TriangleRef triangleRef;
};

#include <impl/HalfEdge.cpp>

#endif  // DRAW_CONVEX_HULL_HALFEDGE_H
