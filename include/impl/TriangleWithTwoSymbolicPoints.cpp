#include <TriangleWithTwoSymbolicPoints.h>

TriangleWithTwoSymbolicPoints::TriangleWithTwoSymbolicPoints(VertexRef p)
    : Triangle(nullptr, p, nullptr) {
  HalfEdgeRef ca = he->getPrev();

  he->setSymbol(HalfEdge::Symbol::Left);
  ca->setSymbol(HalfEdge::Symbol::Right);

  // fixed and arbitrary choice. all half edges are symbolic anyway.
  orientation = unset;
}

bool TriangleWithTwoSymbolicPoints::containsPoint(VertexRef p) const { return *p < *b; }

Triangle::Orientation TriangleWithTwoSymbolicPoints::getOrientation() { return orientation; }

void TriangleWithTwoSymbolicPoints::setOrientation() { orientation = unset; }
Triangle::ChildrenType TriangleWithTwoSymbolicPoints::splitFace(Triangle::VertexRef p) {
  // b is the not symbolic point
  HalfEdgeRef ab = he;
  HalfEdgeRef bc = ab->getNext();
  HalfEdgeRef ca = ab->getPrev();

  TriangleRef abp = new TriangleWithOneSymbolicPoint(HalfEdge::Symbol::Left, b, p);
  TriangleRef pbc = new TriangleWithOneSymbolicPoint(HalfEdge::Symbol::Right, p, b);
  TriangleRef apc = new TriangleWithTwoSymbolicPoints(p);

  // copy twin references to new half edges
  abp->he->setTwin(ab->getTwin());
  pbc->he->getNext()->setTwin(bc->getTwin());
  apc->he->getPrev()->setTwin(ca->getTwin());

  // establish twin reference connections between the new triangles.
  abp->he->getNext()->setTwin(pbc->he);
  pbc->he->getPrev()->setTwin(apc->he->getNext());
  apc->he->setTwin(abp->he->getPrev());

  // delete redundant edges.
  this->deleteEdges();

  Triangle::ChildrenType retval = {abp, pbc, apc};
  return retval;
}