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
  TriangleRef triangle;
  if (qw->hasSymbol() && vw->hasSymbol()) {
    // v (symbolic) -> p -> q (symbolic)
    auto v   = vw->getSymbol();
    auto q   = qw->getSymbol();
    triangle = new TriangleWithTwoSymbolicPoints(p, v, q);
    return triangle->he->getNext();
  } else if (qw->hasSymbol() && !vw->hasSymbol()) {
    // v -> p -> q ia a  {symbol} OR
    auto v   = vw->getOrigin();
    auto q   = qw->getSymbol();
    triangle = new TriangleWithOneSymbolicPoint(q, v, p);

    // return edge pq of new triangle
    return triangle->he->getNext();
  } else if (!qw->hasSymbol() && vw->hasSymbol()) {
    // p -> q -> w is a {symbol} OR p -> w -> q is a {symbol}
    auto v   = vw->getSymbol();
    auto q   = qw->getOrigin();
    triangle = new TriangleWithOneSymbolicPoint(v, p, q);
  } else {
    auto v   = vw->getOrigin();
    auto q   = qw->getOrigin();
    triangle = new Triangle(p, q, v);
  }
  return triangle->he;
}

DelaunayTriangulator::HalfEdgeRef DelaunayTriangulator::flipEdge(HalfEdgeRef vw, VertexRef p) {
  // edge would be legal if twinRef were nullptr or there was no neighbor
  auto wv    = vw->getTwin();
  auto oldQW = wv->getPrev();
  auto oldPV = vw->getPrev();
  auto oldWP = vw->getNext();
  auto oldVQ = wv->getNext();

  auto pq = triangleFactory(p, oldQW, vw);  // half edge pq from triangle pqv
  auto pw = triangleFactory(p, wv, oldQW);  // half edge pw from triangle pwq

  // establish twin reference connections between the new triangles.
  pq->setTwin(pw->getPrev());  // flipped half edge
  pw->getPrev()->setTwin(pq);

  if (oldPV->getTwin() != nullptr) {
    oldPV->getTwin()->setTwin(pq->getPrev());
    pq->getPrev()->setTwin(oldPV->getTwin());
  }

  if (oldWP->getTwin() != nullptr) {
    oldWP->getTwin()->setTwin(pw);
    pw->setTwin(oldWP->getTwin());
  }

  if (oldVQ->getTwin() != nullptr) {
    oldVQ->getTwin()->setTwin(pq->getNext());
    pq->getNext()->setTwin(oldVQ->getTwin());
  }

  if (oldQW->getTwin() != nullptr) {
    oldQW->getTwin()->setTwin(pw->getNext());
    pw->getNext()->setTwin(oldQW->getTwin());
  }

  // set the new triangles as children of both the triangle incident to edge vw & wv;
  // delete edges of triangles pvw && wvq
  TriangleRef pvw = vw->getTriangleRef();
  TriangleRef wvq = wv->getTriangleRef();

  pvw->addChild(pq->getTriangleRef());
  pvw->addChild(pw->getTriangleRef());
  pvw->clearEdges();

  wvq->addChild(pq->getTriangleRef());
  wvq->addChild(pw->getTriangleRef());
  wvq->clearEdges();

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
