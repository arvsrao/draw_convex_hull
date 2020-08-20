#include <DelaunayTriangulator.h>
#include <TriangleWithOneSymbolicPoint.h>
#include <TriangleWithTwoSymbolicPoints.h>

#include <algorithm>
#include <iterator>
#include <random>

DelaunayTriangulator::DelaunayTriangulator() : dag(nullptr) {}

DelaunayTriangulator::DelaunayTriangulator(VertexRefSeq& vertexSeq) {
  VertexRef p0 = *std::max_element(vertexSeq.begin(), vertexSeq.end(),
                                   [&](VertexRef a, VertexRef b) { return *a < *b; });

  // Insert symbolic points before the highest point.
  points.push_back(new Vertex(Vertex::Symbol::Left));
  points.push_back(new Vertex(Vertex::Symbol::Right));
  points.push_back(p0);

  // obtain a time-based seed:
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

  // shuffle vertexSeq
  std::shuffle(vertexSeq.begin(), vertexSeq.end(), std::default_random_engine(seed));

  // copy all vertices except for p0
  for (auto& vertexRef : vertexSeq) {
    // TODO: is it enough to compare the pointers?
    if (vertexRef != p0) points.push_back(vertexRef);
  }

  triangulation.push_back(new TriangleWithTwoSymbolicPoints(points[0], points[2], points[1]));
  dag = new DirectedAcyclicNode<FaceType>(triangulation[0]);

  current = *points.begin();
  std::advance(current, FaceType::NUM_VERTICES_PER_FACE);
}

DelaunayTriangulator::FaceTypeRef DelaunayTriangulator::locatePoint(
    const DelaunayTriangulator::VertexRef vertexRef) {
  auto cur = dag;
  while (cur->hasChildren()) {
    for (auto& child : cur->getChildren()) {
      if (child && child->getFace()->containsPoint(vertexRef)) {
        cur = child;
        break;
      }
    }
  }
  return cur->getFace();
}

void DelaunayTriangulator::operator()() {
  // insert points into triangulation
  while (current != *points.end()) {
    // locate the triangle to split
    FaceTypeRef face = locatePoint(current);
    splitFace(face, current);

    current++;
  }

  // all face references (pointers) in the leaves of dag should be copied into
  // DelaunayTriangulator.triangulation.
}

bool DelaunayTriangulator::isEdgeIllegal(HalfEdge* halfEdge, VertexRef c) {
  VertexRef a = halfEdge->getOrigin();
  VertexRef b = halfEdge->getNext()->getNext()->getOrigin();
  VertexRef d = halfEdge->getTwin()->getNext()->getNext()->getOrigin();

  return false;
}

DelaunayTriangulator::ChildContainerType DelaunayTriangulator::splitFace(FaceTypeRef face,
                                                                         VertexRef p) {
  HalfEdge* ab = face->he;
  HalfEdge* bc = ab->getNext();
  HalfEdge* ca = bc->getNext();
}

DelaunayTriangulator::~DelaunayTriangulator() {
  for (auto& vertex : points) delete vertex;
  for (auto& face : triangulation) delete face;
}
