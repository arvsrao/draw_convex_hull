#include <algorithm>
#include <include/Triangle.h>

DelaunayTriangulator::DelaunayTriangulator(const VertexRefSeq& vertexSeq) : {
  VertexRef p0 = std::max_element(
      vertexSeq.begin(), vertexSeq.end(),
      [&](VertexRef& a, VertexRef& b) { return (a->y == b->y) ? (a->x < b->x) : (a->y < b->y); });
  points.push_back(p0);

  // shuffle vertexSeq
  std::random_shuffle(vertexSeq.begin(), vertexSeq.end());

  // copy all vertices except for p0
  for (auto& vertex : vertexSeq) {
    // TODO: is it enough to compare the pointers?
    if (vertex != p0) points.push_back(vertex);
  }

  triangulation.push_back(new Triangle(p0, Left, Right));

  dag = new DirectedAcyclicNode(triangulation[0]);

}

std::vector<FaceType*> DelaunayTriangulator::operator() {
  return triangulation;
}

DelaunayTriangulator::~DelaunayTriangulator() {
  for (auto& vertex : points) delete vertex;
  for (auto& face : triangulation) delete face;
  delete dag;
}
