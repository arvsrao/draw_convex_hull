#ifndef DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
#define DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H

#include <DirectedAcyclicNode.h>
#include <Triangle.h>
#include <Vertex.h>

#include <vector>

class DelaunayTriangulator {
 public:
  using VertexRef               = Vertex*;
  using VertexRefSeq            = std::vector<VertexRef>;
  using TriangleRef             = Triangle*;
  using HalfEdgeRef             = HalfEdge*;
  using DirectedAcyclicNodeType = DirectedAcyclicNode<Triangle>;
  using DirectedAcyclicNodeRef  = DirectedAcyclicNode<Triangle>*;
  using TriangulationType       = std::vector<TriangleRef>;
  using ChildContainerType =
      std::array<DirectedAcyclicNodeRef, DirectedAcyclicNodeType::MAX_CHILDREN>;

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
  DirectedAcyclicNodeRef dag;

  // pointer to the next Vertex to add to the triangulation
  VertexRef current;

 protected:
  /**
   * @param vertexRef
   * @returng a reference to the face (triangle) that contains the point.
   * It, the triangle, is always in a leaf node.
   */
  DirectedAcyclicNodeRef locatePoint(DirectedAcyclicNodeRef cur, VertexRef vertexRef);

  static bool contains(DirectedAcyclicNodeRef ref, VertexRef p);
  bool isEdgeLegal(HalfEdgeRef he, VertexRef s);
  bool isEdgeLegalNoSymbols(TriangleRef triangleRef, VertexRef s);

  void splitFace(DirectedAcyclicNodeRef ref, VertexRef p);
};

#include <impl/DelaunayTriangulator.cpp>

#endif  // DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
