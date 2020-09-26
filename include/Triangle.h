#ifndef DRAW_CONVEX_HULL_TRIANGLE_H
#define DRAW_CONVEX_HULL_TRIANGLE_H

#include <include/HalfEdge.h>

#include <array>

class Triangle {
 public:
  static const uint8_t NUM_VERTICES_PER_FACE = 3;

  using RingType                 = double;
  using VertexRef                = Vector2D<RingType>*;
  using HalfEdgeRef              = HalfEdge*;
  using TriangleRef              = Triangle*;
  using ChildContainerType       = std::array<TriangleRef, NUM_VERTICES_PER_FACE>;
  using NewEdgeRefsContainerType = std::array<HalfEdgeRef, NUM_VERTICES_PER_FACE + 1>;

  enum Orientation { positive = +1, negative = -1, unset = 0 };

  HalfEdgeRef he;
  VertexRef a, b, c;
  ChildContainerType children;

  explicit Triangle(HalfEdgeRef he);
  Triangle(VertexRef a, VertexRef b, VertexRef c);

  void addChild(TriangleRef child);
  TriangleRef getChild(unsigned idx);

  virtual Orientation getOrientation();

  /** check if point is inside triangle. [includes boundary] */
  virtual bool containsPoint(VertexRef p) const;
  virtual NewEdgeRefsContainerType splitFace(VertexRef p);

  /**
   * VertexRef p is in HalfEdgeRef ref. q is the triangle vertex opposite ref
   * */
  virtual void splitEdgeHelper(HalfEdgeRef ref, VertexRef q, VertexRef p);

  virtual ~Triangle();

 protected:
  Orientation orientation;

  virtual NewEdgeRefsContainerType splitEdge(HalfEdgeRef ref, VertexRef q, VertexRef p);

  virtual void setOrientation();

 private:
  uint numChildren;
};

#include <impl/Triangle.cpp>

#endif  // DRAW_CONVEX_HULL_TRIANGLE_H
