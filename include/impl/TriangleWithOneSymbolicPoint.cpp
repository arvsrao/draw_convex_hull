#include <SquareMatrix.h>
#include <TriangleWithOneSymbolicPoint.h>

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint()
    : Triangle(nullptr, nullptr, nullptr), symbol(HalfEdge::Symbol::None) {
  orientation = unset;
  children    = {nullptr, nullptr, nullptr};
}

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint(HalfEdge::Symbol symbol, VertexRef a,
                                                           VertexRef b)
    : Triangle(a, b, nullptr), symbol(symbol) {
  HalfEdgeRef ca = he->getPrev();

  ca->setSymbol(symbol);

  // fixed and arbitrary choice. all half edges are symbolic anyway.
  orientation = unset;
  children    = {nullptr, nullptr, nullptr};
}

// true if the given vertex is to right of edge ab.
bool TriangleWithOneSymbolicPoint::containsPoint(VertexRef p) const {
  // swap if v is not higher/greater than w
  VertexRef v = a, w = b;
  if (*v < *w) std::swap(v, w);

  std::array<RingType, NUM_VERTICES_PER_FACE * NUM_VERTICES_PER_FACE> mat{
      p->x, p->y, 1,  //
      v->x, v->y, 1,  //
      w->x, w->x, 1,  //
  };

  auto side = SquareMatrix<NUM_VERTICES_PER_FACE, RingType>(mat).det();

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

Triangle::NewEdgeRefsContainerType TriangleWithOneSymbolicPoint::splitEdge(HalfEdgeRef ref,
                                                                           VertexRef q,
                                                                           VertexRef p) {
  splitEdgeHelper(ref, nullptr, p);
  NewEdgeRefsContainerType retVal = {getChild(0)->he->getPrev(), getChild(1)->he->getNext(),
                                     nullptr, nullptr};

  if (ref->getTwin() != nullptr) {
    auto twin     = ref->getTwin();
    auto qPrime   = twin->getPrev()->getOrigin();
    auto neighbor = twin->getTriangleRef();

    // split the neighboring face.
    neighbor->splitEdgeHelper(twin, qPrime, p);
    neighbor->getChild(0)->he->setTwin(this->getChild(1)->he);
    neighbor->getChild(1)->he->setTwin(this->getChild(0)->he);

    this->getChild(0)->he->setTwin(neighbor->getChild(1)->he);
    this->getChild(1)->he->setTwin(neighbor->getChild(0)->he);

    // should return new edges.
    retVal[3] = neighbor->getChild(0)->he->getPrev();
    retVal[4] = neighbor->getChild(1)->he->getNext();
  }

  return retVal;
}

// if called then it is known that p is in halfedge he, and the vertex opposite is
// the symbol
void TriangleWithOneSymbolicPoint::splitEdgeHelper(HalfEdgeRef ref, VertexRef q, VertexRef p) {
  auto v = ref->getOrigin();
  auto w = ref->getNext()->getOrigin();

  auto vpq = new TriangleWithOneSymbolicPoint(symbol, v, p);
  auto pwq = new TriangleWithOneSymbolicPoint(symbol, p, w);

  // establish twin reference connections between the new triangles.
  auto pw = pwq->he;
  auto wq = pw->getNext();
  auto qp = pw->getPrev();

  auto vp = vpq->he;
  auto pq = vp->getNext();
  auto qv = vp->getPrev();

  // establish twin reference connections between the new triangles.
  pq->setTwin(qp);

  if (ref->getPrev()->getTwin() != nullptr) {
    qv->setTwin(ref->getPrev()->getTwin());
    ref->getPrev()->getTwin()->setTwin(qv);
  }

  if (ref->getNext()->getTwin() != nullptr) {
    wq->setTwin(ref->getNext()->getTwin());
    ref->getNext()->getTwin()->setTwin(wq);
  }

  addChild(vpq);
  addChild(pwq);

  clearEdges();
}

Triangle::NewEdgeRefsContainerType TriangleWithOneSymbolicPoint::splitFace(VertexRef p) {
  HalfEdgeRef ab = he;
  HalfEdgeRef bc = ab->getNext();
  HalfEdgeRef ca = ab->getPrev();

  // ab/he is the only edge of the triangle that does not have a symbol.
  if (he->isVertexInHalfEdge(p))
    return splitEdge(he, nullptr, p);
  else {
    TriangleRef abp = new Triangle(a, b, p);
    TriangleRef pbc = new TriangleWithOneSymbolicPoint(symbol, p, b);
    TriangleRef apc = new TriangleWithOneSymbolicPoint(symbol, a, p);

    // copy twin references to new half edges
    abp->he->setTwin(ab->getTwin());
    if (ab->getTwin() != nullptr) ab->getTwin()->setTwin(abp->he);

    pbc->he->getNext()->setTwin(bc->getTwin());
    if (bc->getTwin() != nullptr) bc->getTwin()->setTwin(pbc->he->getNext());

    apc->he->getPrev()->setTwin(ca->getTwin());
    if (ca->getTwin() != nullptr) ca->getTwin()->setTwin(apc->he->getPrev());

    // establish twin reference connections between the new triangles.
    abp->he->getNext()->setTwin(pbc->he);
    pbc->he->setTwin(abp->he->getNext());

    pbc->he->getPrev()->setTwin(apc->he->getNext());
    apc->he->getNext()->setTwin(pbc->he->getPrev());

    apc->he->setTwin(abp->he->getPrev());
    abp->he->getPrev()->setTwin(apc->he);

    addChild(abp);
    addChild(pbc);
    addChild(apc);

    clearEdges();

    return {abp->he, pbc->he->getNext(), apc->he->getPrev(), nullptr};
  }
}
