#include <algorithm>

DelaunayTriangulator::DelaunayTriangulator(const VertexRefSeq& vertexSeq) : {
  VertexRef p0 =
      std::max_element(vertexSeq.begin(), vertexSeq.end(),
                       [&](VertexRef& a, VertexRef& b) { return (a->y < b->y) || (a->x < b->x); });
  points.push_back(p0);

  // shuffle vertexSeq
  std::random_shuffle(vertexSeq.begin(), vertexSeq.end());

  // copy all vertices except for p0
  for (auto& vertex : vertexSeq) {
    // TODO: is it enough to compare the pointers?
    if (vertex != p0) points.push_back(vertex);
  }
}

DelaunayTriangulator::operator() { dag = new DirectedAcyclicNode(); }

DelaunayTriangulator::~DelaunayTriangulator() {
  for (auto& vertex : points) delete vertex;
  delete dag;
}