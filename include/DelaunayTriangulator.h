#ifndef DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
#define DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H

#include <DirectedAcyclicNode.h>
#include <Triangle.h>
#include <Vector2D.h>

#include <vector>

class DelaunayTriangulator {
 public:
  using VertexRef         = Vector2D<double>*;
  using VertexRefSeq      = std::vector<VertexRef>;
  using FaceType          = Triangle<double>;
  using TriangulationType = DirectedAcyclicNode<FaceType>;

  DelaunayTriangulator(VertexRefSeq& vertexSeq);
  ~DelaunayTriangulator();

  TriangulationType& operator()();

 private:
  // points to be triangulated
  VertexRefSeq points;

  // the triangulation
  TriangulationType dag;
};

#include <impl/DelaunayTriangulator.cpp>

#endif  // DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
