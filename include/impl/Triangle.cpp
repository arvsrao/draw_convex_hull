#include <Triangle.h>
#include <include/SquareMatrix.h>

#include <array>

Triangle::Triangle(VertexRef a, VertexRef b, VertexRef c) : a(a), b(b), c(c) {
  he      = new HalfEdge(a, nullptr, nullptr, nullptr);
  auto bc = new HalfEdge(b, nullptr, nullptr, nullptr);
  auto ca = new HalfEdge(c, nullptr, nullptr, nullptr);

  he->setPrev(ca);
  he->setNext(bc);

  bc->setPrev(he);
  bc->setNext(ca);

  ca->setPrev(bc);
  ca->setNext(he);

  he->setTriangleRef(this);
  bc->setTriangleRef(this);
  ca->setTriangleRef(this);

  orientation = unset;
  numChildren = 0;
}

Triangle::Triangle(HalfEdgeRef he) : he(he), a(nullptr), b(nullptr), c(nullptr) {
  if (he) a = he->getOrigin();
  if (a && he->getNext()) b = he->getNext()->getOrigin();
  if (b) c = he->getNext()->getNext()->getOrigin();

  he->setTriangleRef(this);
  he->getNext()->setTriangleRef(this);
  he->getNext()->getNext()->setTriangleRef(this);

  orientation = unset;
  numChildren = 0;
}

template <typename T, unsigned N>
class nonZeroElementsIterator {
 public:
  std::array<T, N> mu;

  // the index of the most recent encountered nonzero element of mu.
  int currentIdx;

  // the largest index of all nonzero entries of mu
  int endIdx;

  nonZeroElementsIterator(std::array<T, N>& mu) : mu(mu), currentIdx(-1), endIdx(N - 1) {
    while (!mu[endIdx]) --endIdx;
  }

  // has iterator reached the last nonzero element
  bool end() { return currentIdx >= endIdx; }

  T operator()() {
    if (!end())
      while (!mu[++currentIdx]) continue;
    return mu[currentIdx];
  }
};

bool Triangle::containsPoint(VertexRef p) const {
  // compute barycentric coordinates, \eta, of point p relative to this
  // triangle. As a triple barycentric coordinates would be a multiple of
  // any vector normal to the plane determined by
  // v = (a.x - p.x, b.x - p.x, c.x -p.x) and w = (a.y - p.y, b.y - p.y, c.y - p.y). Let \mu be
  // the cross product of v & w, then point p is inside triangle iff there is a \lambda \in
  // \mathbb{R}^{*} (\lambda can be NOT zero) s.t. \eta = \lambda & \mu. \eta is a unit 3D vector
  // s.t. all of its coordinates are non-negative ( lying on an edge counts ). Dividing \eta by
  // \lambda implies that it is enough to check that coordinates of \mu are all
  // either non-negative or non-positive. So,
  //                \mu = (1,1,0) & (-1,-1,0) are IN
  // but,
  //              \mu = (1,-1,0) & (-1, 1, 1) are OUT
  //
  //                  [   i          j              k   ]
  //       \mu = det  [ a.x - p.x  b.x - p.x   c.x -p.x ]
  //                  [ a.y - p.y  b.y - p.y   c.y -p.y ]

  std::array<RingType, NUM_VERTICES_PER_FACE> mu = {
      (a->x - p->x) * (b->y - p->y) - (a->y - p->y) * (b->x - p->x),
      (a->y - p->y) * (c->x - p->x) - (a->x - p->x) * (c->y - p->y),
      (b->x - p->x) * (c->y - p->y) - (b->y - p->y) * (c->x - p->x)};

  // compare only nonzero entries of mu
  RingType firstNonZeroElement, next;
  auto nextNonZeroElement = nonZeroElementsIterator<RingType, NUM_VERTICES_PER_FACE>(mu);
  firstNonZeroElement     = nextNonZeroElement();

  while (!nextNonZeroElement.end()) {
    next = nextNonZeroElement();
    if (firstNonZeroElement > 0 != next > 0) return false;
  }

  return true;
}

void Triangle::addChild(TriangleRef child) {
  children[numChildren % NUM_VERTICES_PER_FACE] = child;
  numChildren++;
}

Triangle::TriangleRef Triangle::getChild(unsigned int idx) {
  return children.at(idx % NUM_VERTICES_PER_FACE);
}

Triangle::Orientation Triangle::getOrientation() {
  if (orientation == unset) setOrientation();
  return orientation;
}

void Triangle::setOrientation() {
  std::array<RingType, 9> mat = {
      a->x, a->y, 1,  //
      b->x, b->y, 1,  //
      c->x, c->y, 1,  //
  };

  orientation = SquareMatrix<3, RingType>(mat).det() > 0 ? positive : negative;
}

Triangle::~Triangle() {
  // Triangle owns all the half edges of the face.
  delete he;
}

Triangle::NewEdgeRefsContainerType Triangle::splitEdge(HalfEdgeRef ref, VertexRef q, VertexRef p) {
  splitEdgeHelper(ref, q, p);

  NewEdgeRefsContainerType retVal = {getChild(0)->he->getPrev(), getChild(1)->he->getNext(),
                                     nullptr, nullptr};

  if (ref->getTwin() != nullptr) {
    auto twin     = ref->getTwin();
    auto qPrime   = twin->getPrev()->getOrigin();
    auto neighbor = twin->getTriangleRef();

    // split the neighboring face & set twin refs on the new half edges
    neighbor->splitEdgeHelper(twin, qPrime, p);
    neighbor->getChild(0)->he->setTwin(getChild(1)->he);
    getChild(1)->he->setTwin(neighbor->getChild(0)->he);

    neighbor->getChild(1)->he->setTwin(getChild(0)->he);
    getChild(0)->he->setTwin(neighbor->getChild(1)->he);

    delete twin;

    // should return new edges.
    retVal[3] = neighbor->getChild(0)->he->getPrev();
    retVal[4] = neighbor->getChild(1)->he->getNext();
  }

  delete he;

  // return new edges.
  return retVal;
}

void Triangle::splitEdgeHelper(HalfEdgeRef ref, VertexRef q, VertexRef p) {
  auto v = ref->getOrigin();
  auto w = ref->getNext()->getOrigin();

  auto vpq = new Triangle(v, p, q);
  auto pwq = new Triangle(p, w, q);

  // establish twin reference connections between the new triangles.
  vpq->he->getNext()->setTwin(pwq->he->getPrev());

  vpq->he->getPrev()->setTwin(ref->getPrev()->getTwin());
  ref->getPrev()->getTwin()->setTwin(vpq->he->getPrev());

  pwq->he->getNext()->setTwin(ref->getNext()->getTwin());
  ref->getNext()->getTwin()->setTwin(pwq->he->getNext());

  addChild(vpq);
  addChild(pwq);
}

Triangle::NewEdgeRefsContainerType Triangle::splitFace(VertexRef p) {
  HalfEdgeRef ab = this->he;
  HalfEdgeRef bc = ab->getNext();
  HalfEdgeRef ca = ab->getPrev();

  if (ab->isVertexInHalfEdge(p))
    return splitEdge(ab, c, p);
  else if (bc->isVertexInHalfEdge(p))
    return splitEdge(bc, a, p);
  else if (ca->isVertexInHalfEdge(p))
    return splitEdge(ca, b, p);
  else {
    auto abp = new Triangle(this->a, this->b, p);
    auto bcp = new Triangle(this->b, this->c, p);
    auto cap = new Triangle(this->c, this->a, p);

    // copy twin references to new half edges and point the neighboring edges to the
    // new edges
    abp->he->setTwin(ab->getTwin());
    ab->getTwin()->setTwin(abp->he);

    bcp->he->setTwin(bc->getTwin());
    bc->getTwin()->setTwin(bcp->he);

    cap->he->setTwin(ca->getTwin());
    ca->getTwin()->setTwin(cap->he);

    // establish twin reference connections between the new triangles.
    abp->he->getNext()->setTwin(bcp->he->getPrev());
    bcp->he->getPrev()->setTwin(abp->he->getNext());

    bcp->he->getNext()->setTwin(cap->he->getPrev());
    cap->he->getPrev()->setTwin(bcp->he->getNext());

    cap->he->getNext()->setTwin(abp->he->getPrev());
    abp->he->getPrev()->setTwin(cap->he->getNext());

    children = {abp, bcp, cap};
    delete he;

    // return (new) edges to the triangle that was split
    return {abp->he, bcp->he, cap->he, nullptr};
  }
}
