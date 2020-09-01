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
  dag = new DirectedAcyclicNodeType(triangulation[0]);

  current = *points.begin();
  std::advance(current, Triangle::NUM_VERTICES_PER_FACE);
}

DelaunayTriangulator::TriangleRef DelaunayTriangulator::locatePoint(
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
    TriangleRef face = locatePoint(current);
    splitFace(face, current);

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
  auto twinRef = he->getTwin();
  if (!twinRef || !twinRef->getTriangleRef()) return true;

  // c can be symbolic. it is the vertex of the half edge in the neighboring triangle
  VertexRef c = twinRef->getNext()->getNext()->getOrigin();
  if (c->isSymbol() || he->hasSymbol()) {
    return c->getSymbol() <
           std::min(he->getOrigin()->getSymbol(), twinRef->getOrigin()->getSymbol());
  } else
    return isEdgeLegalNoSymbols(he, s);
}

/**
 * The predicate for the case there no are symbolic points in either the half edge or
 * the adjacent triangle.
 * */
bool DelaunayTriangulator::isEdgeLegalNoSymbols(HalfEdgeRef he, VertexRef s) {
  TriangleRef triangleRef = he->getTwin()->getTriangleRef();
  VertexRef a = triangleRef->a, b = triangleRef->b, c = triangleRef->c;

  std::array<Vertex::RingType, 16> mat = {
      s->x, s->y, (s->x) * (s->x) + (s->y) * (s->y), 1,  //
      a->x, a->y, (a->x) * (a->x) + (a->y) * (a->y), 1,  //
      b->x, b->y, (b->x) * (b->x) + (b->y) * (b->y), 1,  //
      c->x, c->y, (c->x) * (c->x) + (c->y) * (c->y), 1,  //
  };

  // correct the determinant for triangle orientation
  return triangleRef->getOrientation() * SquareMatrix<4, Vertex::RingType>(mat).det() < 0;
}

DelaunayTriangulator::ChildContainerType DelaunayTriangulator::splitFace(TriangleRef face,
                                                                         VertexRef p) {
  HalfEdge* ab = face->he;
  HalfEdge* bc = ab->getNext();
  HalfEdge* ca = bc->getNext();
}

DelaunayTriangulator::~DelaunayTriangulator() {
  for (auto& vertex : points) delete vertex;
  for (auto& face : triangulation) delete face;
}
