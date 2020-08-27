#include <SquareMatrix.h>

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint()
    : Triangle(nullptr, nullptr, nullptr), symbol(Vertex::Symbol::None) {}

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint(Vertex::Symbol symbol, VertexRef a,
                                                           VertexRef b)
    : Triangle(a, b, nullptr), symbol(symbol) {}

Triangle::HalfEdgeRef TriangleWithOneSymbolicPoint::halfEdgeContainsPoint(Triangle::VertexRef p) {
  return he->isVertexInHalfEdge(p) ? he : nullptr;
}

// true if the given vertex is to right of edge ab.
bool TriangleWithOneSymbolicPoint::containsPoint(VertexRef p) const {
  auto a = he->getOrigin();
  auto b = he->getNext()->getOrigin();

  // swap if a is not higher/greater than b
  if (*a < *b) std::swap(a, b);

  std::array<Vertex::RingType, NUM_VERTICES_PER_FACE * NUM_VERTICES_PER_FACE> mat{
      p->x, p->y, 1,  //
      a->x, a->y, 1,  //
      b->x, b->x, 1,  //
  };

  auto side = SquareMatrix<NUM_VERTICES_PER_FACE, Vertex::RingType>(mat).det();

  switch (symbol) {
    case Vertex::Left:
      return side < 0 && (*p < *a) && (*p > *b);
    case Vertex::Right:
      return side > 0 && (*p < *a) && (*p > *b);
    default:
      return false;  // possibly identify if p is in the edge
  }
}

Triangle::ChildrenType TriangleWithOneSymbolicPoint::splitFace(Triangle::VertexRef p) {
  HalfEdgeRef ab = this->he;
  HalfEdgeRef bc = ab->getNext();
  HalfEdgeRef ca = ab->getPrev();

  TriangleRef abp = new Triangle(this->a, this->b, p);
  TriangleRef pbc = new TriangleWithOneSymbolicPoint(symbol, this->b, this->c);
  TriangleRef apc = new TriangleWithOneSymbolicPoint(symbol, this->a, p);

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
