#include <TriangleWithTwoSymbolicPoints.h>

TriangleWithTwoSymbolicPoints::TriangleWithTwoSymbolicPoints(VertexRef p)
    : Triangle(nullptr, p, nullptr) {
  HalfEdgeRef ca = he->getPrev();

  he->setSymbol(HalfEdge::Symbol::Left);
  ca->setSymbol(HalfEdge::Symbol::Right);

  // fixed and arbitrary choice. all half edges are symbolic anyway.
  orientation = negative;
  children    = {nullptr, nullptr, nullptr};
}

TriangleWithTwoSymbolicPoints::TriangleWithTwoSymbolicPoints(VertexRef p, HalfEdge::Symbol a,
                                                             HalfEdge::Symbol b)
    : Triangle(nullptr, p, nullptr) {
  he->setSymbol(a);
  he->getPrev()->setSymbol(b);

  // fixed and arbitrary choice. all half edges are symbolic anyway.
  orientation = he->getSymbol() == HalfEdge::Left ? negative : positive;
  children    = {nullptr, nullptr, nullptr};
}

bool TriangleWithTwoSymbolicPoints::containsPoint(VertexRef p) const { return *p < *b; }

Triangle::Orientation TriangleWithTwoSymbolicPoints::getOrientation() { return orientation; }

void TriangleWithTwoSymbolicPoints::setOrientation() { orientation = unset; }

Triangle::NewEdgeRefsContainerType TriangleWithTwoSymbolicPoints::splitFace(Triangle::VertexRef p) {
  // b is not a symbolic point
  HalfEdgeRef ab = he;
  HalfEdgeRef bc = ab->getNext();
  HalfEdgeRef ca = ab->getPrev();

  TriangleRef bpa = new TriangleWithOneSymbolicPoint(ab->getSymbol(), b, p);
  TriangleRef pbc = new TriangleWithOneSymbolicPoint(ca->getSymbol(), p, b);
  TriangleRef apc = new TriangleWithTwoSymbolicPoints(p, ab->getSymbol(), ca->getSymbol());

  // copy twin references to new half edges
  auto newAB = bpa->he->getPrev();
  newAB->setTwin(ab->getTwin());
  if (ab->getTwin() != nullptr) ab->getTwin()->setTwin(newAB);

  pbc->he->getNext()->setTwin(bc->getTwin());
  if (bc->getTwin() != nullptr) bc->getTwin()->setTwin(pbc->he->getNext());

  apc->he->getPrev()->setTwin(ca->getTwin());
  if (ca->getTwin() != nullptr) ca->getTwin()->setTwin(apc->he->getPrev());

  // establish twin reference connections between the new triangles.
  newAB->getNext()->setTwin(pbc->he);
  pbc->he->setTwin(newAB->getNext());

  pbc->he->getPrev()->setTwin(apc->he->getNext());
  apc->he->getNext()->setTwin(pbc->he->getPrev());

  apc->he->setTwin(newAB->getPrev());
  newAB->getPrev()->setTwin(apc->he);

  addChild(bpa);
  addChild(pbc);
  addChild(apc);

  clearEdges();

  return {newAB->getNext(), pbc->he->getPrev(), apc->he, nullptr};
}
