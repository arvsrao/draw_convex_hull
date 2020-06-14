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
  using TriangulationType = std::vector<FaceType*>;

  DelaunayTriangulator(VertexRefSeq& vertexSeq);
  ~DelaunayTriangulator();

  TriangulationType& operator()();

 private:
  enum SymbolicPoints { Left = -1, Right = -2 };

  // points to be triangulated
  VertexRefSeq points;

  // the triangulation
  TriangulationType triangulation;

  // a point location data structure with pointers
  // into the triangulation
  DirectedAcyclicNode<FaceType> dag;
};

#include <impl/DelaunayTriangulator.cpp>

#endif  // DRAW_CONVEX_HULL_DELAUNAYTRIANGULATOR_H
