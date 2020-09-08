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

  // Do Not insert symbolic points before the highest point.
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

  triangulation.push_back(new TriangleWithTwoSymbolicPoints(points[0]));
  dag = new DirectedAcyclicNodeType(triangulation[0]);

  current = *points.begin();
  std::advance(current, Triangle::NUM_VERTICES_PER_FACE);
}

bool DelaunayTriangulator::contains(DirectedAcyclicNodeRef ref, VertexRef p) {
  return ref->hasFace() && ref->getFace()->containsPoint(p);
}

DelaunayTriangulator::DirectedAcyclicNodeRef DelaunayTriangulator::locatePoint(
    DirectedAcyclicNodeRef cur, const VertexRef p) {
  if (cur != nullptr && contains(cur, p)) {
    for (auto& child : cur->getChildren()) {
      auto retval = locatePoint(child, p);
      if (retval != nullptr) return retval;
    }
    return cur;
  }
  return nullptr;
}

void DelaunayTriangulator::operator()() {
  // insert points into triangulation
  while (current != *points.end()) {
    // locate the triangle to split
    auto dagNodeRef = locatePoint(dag, current);

    // split the face inside the node.
    if (dagNodeRef != nullptr) splitFace(dagNodeRef, current);

    current++;
  }

  // all face references (pointers) in the leaves of dag should be copied into
  // DelaunayTriangulator.triangulation.
}

/** By the time this function is called the c */
bool DelaunayTriangulator::isEdgeLegal(HalfEdgeRef he, VertexRef s) {
  // if there is no triangle on the other side of the edge then no point in
  // flipping it. Just return false. Conveniently, this covers the case when an
  // edge has two symbolic endpoints.
  HalfEdgeRef twinRef = he->getTwin();
  if (!twinRef || !twinRef->getTriangleRef()) return true;

  // c can be symbolic. it is the vertex of the half edge in the neighboring triangle
  HalfEdgeRef ca = twinRef->getPrev();
  if (ca->hasSymbol() || he->hasSymbol()) {
    return ca->getSymbol() < std::min(he->getSymbol(), twinRef->getSymbol());
  } else
    return isEdgeLegalNoSymbols(twinRef->getTriangleRef(), s);
}

/**
 * The predicate for the case there no are symbolic points in either the half edge or
 * the adjacent triangle.
 * */
bool DelaunayTriangulator::isEdgeLegalNoSymbols(TriangleRef triangleRef, VertexRef s) {
  VertexRef a = triangleRef->a, b = triangleRef->b, c = triangleRef->c;

  std::array<Vertex::RingType, 16> mat = {
      s->x, s->y, (s->x) * (s->x) + (s->y) * (s->y), 1,  //
      a->x, a->y, (a->x) * (a->x) + (a->y) * (a->y), 1,  //
      b->x, b->y, (b->x) * (b->x) + (b->y) * (b->y), 1,  //
      c->x, c->y, (c->x) * (c->x) + (c->y) * (c->y), 1,  //
  };

  // correct the determinant for triangle orientation
  bool inside_circle = SquareMatrix<4, Vertex::RingType>(mat).det() > 0;
  return (triangleRef->getOrientation() == Triangle::negative) ? !inside_circle : inside_circle;
}

void DelaunayTriangulator::splitFace(DirectedAcyclicNodeRef dagRef, VertexRef p) {
  auto children = dagRef->getFace()->splitFace(p);

  dagRef->addChild(children[0]);
  dagRef->addChild(children[1]);
  dagRef->addChild(children[2]);
}

DelaunayTriangulator::~DelaunayTriangulator() {
  for (auto& vertex : points) delete vertex;
  for (auto& face : triangulation) delete face;
}
