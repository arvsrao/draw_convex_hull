#ifndef DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
#define DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H

#include <Triangle.h>

#include <vector>

class DelaunayTriangulator {
 public:
  using RingType          = Triangle::RingType;
  using VertexRef         = Triangle::VertexRef;
  using VertexRefSeq      = std::vector<VertexRef>;
  using TriangleRef       = Triangle*;
  using HalfEdgeRef       = HalfEdge*;
  using TriangulationType = std::vector<TriangleRef>;

  DelaunayTriangulator();
  DelaunayTriangulator(VertexRefSeq& vertexSeq);
  ~DelaunayTriangulator();

  /**
   * Triangulate the point set given at the time of construction. Along the way
   * a point location data stucture is built ( DelaunayTriangulator.dag ). At the end
   * all face references (pointsrs) in the leafs of dag should be copied into
   * DelaunayTriangulator.triangulation.
   */
  void operator()();

 private:
  // points to be triangulated
  VertexRefSeq points;

  // the triangulation
  TriangulationType triangulation;

  // a point location data structure with pointers
  // into the triangulation
  TriangleRef dag;

 protected:
  /**
   * @param vertexRef
   * @returng a reference to the face (triangle) that contains the point.
   * It, the triangle, is always in a leaf node.
   */
  TriangleRef locatePoint(TriangleRef cur, VertexRef vertexRef);

  bool isEdgeLegal(HalfEdgeRef he, VertexRef s);
  bool isEdgeLegalNoSymbols(TriangleRef triangleRef, VertexRef s);
  void legalizeEdge(HalfEdgeRef edge, VertexRef p);
  HalfEdgeRef flipEdge(HalfEdgeRef edge, VertexRef p);

 private:
  HalfEdgeRef triangleFactory(VertexRef p, HalfEdgeRef qw, HalfEdgeRef vw);
};

#include <impl/DelaunayTriangulator.cpp>

#endif  // DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
