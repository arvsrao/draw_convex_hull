#include <SquareMatrix.h>
#include <TriangleWithOneSymbolicPoint.h>

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint()
    : Triangle(nullptr, nullptr, nullptr), symbol(HalfEdge::Symbol::None) {
  orientation = unset;
}

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint(HalfEdge::Symbol symbol, VertexRef a,
                                                           VertexRef b)
    : Triangle(a, b, nullptr), symbol(symbol) {
  HalfEdgeRef ca = he->getPrev();

  ca->setSymbol(symbol);

  // fixed and arbitrary choice. all half edges are symbolic anyway.
  orientation = unset;
}

Triangle::HalfEdgeRef TriangleWithOneSymbolicPoint::halfEdgeContainsPoint(Triangle::VertexRef p) {
  return he->isVertexInHalfEdge(p) ? he : nullptr;
}

// true if the given vertex is to right of edge ab.
bool TriangleWithOneSymbolicPoint::containsPoint(VertexRef p) const {
  // swap if v is not higher/greater than w
  VertexRef v = a, w = b;
  if (*v < *w) std::swap(v, w);

  std::array<Vertex::RingType, NUM_VERTICES_PER_FACE * NUM_VERTICES_PER_FACE> mat{
      p->x, p->y, 1,  //
      v->x, v->y, 1,  //
      w->x, w->x, 1,  //
  };

  auto side = SquareMatrix<NUM_VERTICES_PER_FACE, Vertex::RingType>(mat).det();

  switch (symbol) {
    case HalfEdge::Left:
      return side < 0 && (*p < *v) && (*p > *w);
    case HalfEdge::Right:
      return side > 0 && (*p < *v) && (*p > *w);
    default:
      return false;  // possibly identify if p is in the edge
  }
}

void TriangleWithOneSymbolicPoint::setOrientation() {
  switch (symbol) {
    case HalfEdge::Left:
      orientation = (*a < *b) ? positive : negative;
      break;
    case HalfEdge::Right:
      orientation = (*a < *b) ? negative : positive;
      break;
    default:
      orientation = unset;  // nothing
  }
}

Triangle::Orientation TriangleWithOneSymbolicPoint::getOrientation() {
  if (orientation == unset) setOrientation();
  return orientation;
}

Triangle::ChildrenType TriangleWithOneSymbolicPoint::splitFace(VertexRef p) {
  HalfEdgeRef ab = he;
  HalfEdgeRef bc = ab->getNext();
  HalfEdgeRef ca = ab->getPrev();

  TriangleRef abp = new Triangle(a, b, p);
  TriangleRef pbc = new TriangleWithOneSymbolicPoint(symbol, p, b);
  TriangleRef apc = new TriangleWithOneSymbolicPoint(symbol, a, p);

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
