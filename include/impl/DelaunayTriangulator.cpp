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
  dag = triangulation[0];
}

DelaunayTriangulator::TriangleRef DelaunayTriangulator::locatePoint(TriangleRef cur,
                                                                    const VertexRef p) {
  if (cur != nullptr && cur->containsPoint(p)) {
    for (unsigned idx = 0; idx < Triangle::NUM_VERTICES_PER_FACE; idx++) {
      auto retval = locatePoint(cur->getChild(idx), p);
      if (retval != nullptr) return retval;
    }
    return cur;
  }
  return nullptr;
}

void DelaunayTriangulator::operator()() {
  // insert points into triangulation
  for (int idx = 1; idx < points.size(); idx++) {
    // locate the triangle to split
    auto triangleRef = locatePoint(dag, points[idx]);

    // split the face inside the node.
    if (triangleRef != nullptr) {
      // split should return references edges of triangle(s) that were split
      auto edges = triangleRef->splitFace(points[idx]);
      for (auto& edge : edges) legalizeEdge(edge, points[idx]);
    }
  }

  // all face references (pointers) in the leaves of dag should be copied into
  // DelaunayTriangulator.triangulation.
}

DelaunayTriangulator::HalfEdgeRef DelaunayTriangulator::triangleFactory(VertexRef p, HalfEdgeRef qw,
                                                                        HalfEdgeRef vw) {
  // the flipped edge goes from p to q
  // edge goes from v to w
  // nonSymbolicPoint is never symbolic
  TriangleRef newTriangle;
  if (qw->hasSymbol() && vw->hasSymbol()) {
    // v (symbolic) -> p -> q (symbolic)
    newTriangle = new TriangleWithTwoSymbolicPoints(p, vw->getSymbol(), qw->getSymbol());
    return newTriangle->he->getNext();
  } else if (qw->hasSymbol() && !vw->hasSymbol()) {
    // v -> p -> q ia a  {symbol}
    newTriangle = new TriangleWithOneSymbolicPoint(qw->getSymbol(), vw->getOrigin(), p);
    return newTriangle->he->getNext();
  } else if (!qw->hasSymbol() && vw->hasSymbol()) {
    // p -> q -> w is a {symbol}
    newTriangle = new TriangleWithOneSymbolicPoint(vw->getSymbol(), p, qw->getOrigin());
  } else {
    newTriangle = new Triangle(p, qw->getOrigin(), vw->getOrigin());
  }
  return newTriangle->he;
}

DelaunayTriangulator::HalfEdgeRef DelaunayTriangulator::flipEdge(HalfEdgeRef vw, VertexRef p) {
  // edge would be legal if twinRef were nullptr or there was no neighbor
  auto wv = vw->getTwin();
  auto qw = wv->getPrev();

  //  auto pqv = triangleFactory(nonSymbolicPoint, q, edge->getOrigin());
  //  auto pwq = triangleFactory(nonSymbolicPoint, twinRef->getOrigin(), q);
  auto pq = triangleFactory(p, qw, vw);  // half edge pq from triangle pqv
  auto pw = triangleFactory(p, wv, qw);  // half edge pw from triangle pwq

  // establish twin reference connections between the new triangles.
  pq->setTwin(pw->getPrev());  // flipped half edge
  pw->getPrev()->setTwin(pq);

  vw->getPrev()->getTwin()->setTwin(pq->getPrev());
  pq->getPrev()->setTwin(vw->getPrev()->getTwin());

  vw->getNext()->getTwin()->setTwin(pw);
  pw->setTwin(vw->getNext()->getTwin());

  wv->getNext()->getTwin()->setTwin(pq->getNext());
  pq->getNext()->setTwin(wv->getNext()->getTwin());

  qw->getTwin()->setTwin(pw->getNext());
  pw->getNext()->setTwin(qw->getTwin());

  // set new triangles as children of the triangles incident to edge and twinRef
  vw->getTriangleRef()->addChild(pq->getTriangleRef());
  vw->getTriangleRef()->addChild(pw->getTriangleRef());
  wv->getTriangleRef()->addChild(pq->getTriangleRef());
  wv->getTriangleRef()->addChild(pw->getTriangleRef());

  // delete edges of triangles pvw && wvq
  delete vw;
  delete wv;

  // return flipped edge pq
  return pq;
}

void DelaunayTriangulator::legalizeEdge(HalfEdgeRef edge, VertexRef p) {
  if (!isEdgeLegal(edge, p)) {
    auto pq = flipEdge(edge, p);
    legalizeEdge(pq->getNext(), p);
    legalizeEdge(pq->getTwin()->getPrev(), p);
  }
}

/** By the time this function is called the c */
bool DelaunayTriangulator::isEdgeLegal(HalfEdgeRef he, VertexRef s) {
  // if there is no triangle on the other side of the edge then no point in
  // flipping it. Just return false. Conveniently, this covers the case when an
  // edge has two symbolic endpoints.
  HalfEdgeRef twinRef = he->getTwin();
  if (twinRef == nullptr || twinRef->getTriangleRef() == nullptr) return true;

  // c can be symbolic. it is the vertex of the half edge in the neighboring triangle
  HalfEdgeRef ca = twinRef->getPrev();
  if (twinRef->hasSymbol() || ca->hasSymbol() || he->hasSymbol())
    return ca->getSymbol() < std::min(he->getSymbol(), twinRef->getSymbol());
  else
    return isEdgeLegalNoSymbols(twinRef->getTriangleRef(), s);
}

/**
 * The predicate for the case there no are symbolic points in either the half edge or
 * the adjacent triangle.
 * */
bool DelaunayTriangulator::isEdgeLegalNoSymbols(TriangleRef triangleRef, VertexRef s) {
  VertexRef a = triangleRef->a, b = triangleRef->b, c = triangleRef->c;

  std::array<RingType, 16> mat = {
      s->x, s->y, (s->x) * (s->x) + (s->y) * (s->y), 1,  //
      a->x, a->y, (a->x) * (a->x) + (a->y) * (a->y), 1,  //
      b->x, b->y, (b->x) * (b->x) + (b->y) * (b->y), 1,  //
      c->x, c->y, (c->x) * (c->x) + (c->y) * (c->y), 1,  //
  };

  // correct the determinant for triangle orientation
  bool inside_circle = SquareMatrix<4, RingType>(mat).det() > 0;
  return (triangleRef->getOrientation() == Triangle::negative) ? !inside_circle : inside_circle;
}

DelaunayTriangulator::~DelaunayTriangulator() {
  for (auto& vertex : points) delete vertex;
  for (auto& face : triangulation) delete face;
}
