#include <array>

const uint8_t Triangle::NUM_VERTICES_PER_FACE = 3;

Triangle::Triangle(VertexRef _a, VertexRef _b, VertexRef _c) : a(_a), b(_b), c(_c) {
  he             = new HalfEdge(a);
  HalfEdgeRef bc = new HalfEdge(b);
  HalfEdgeRef ca = new HalfEdge(c);

  he->setPrev(ca);
  he->setNext(bc);

  bc->setPrev(he);
  bc->setNext(ca);

  ca->setPrev(bc);
  ca->setNext(he);
}

Triangle::Triangle(HalfEdgeRef _he) : he(_he), a(nullptr), b(nullptr), c(nullptr) {
  if (he) a = he->getOrigin();
  if (a && he->getNext()) b = he->getNext()->getOrigin();
  if (b) c = he->getNext()->getNext()->getOrigin();
}

template <typename T, unsigned N>
class nonZeroElementsIterator {
 public:
  std::array<T, N> mu;

  // the index of the most recent encountered nonzero element of mu.
  int currentIdx;

  // the largest index of all nonzero entries of mu
  int endIdx;

  nonZeroElementsIterator(std::array<T, N>& _mu) : mu(_mu), currentIdx(-1), endIdx(N - 1) {
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

bool Triangle::containsPoint(const VertexRef p) const {
  // compute barycentric coordinates, \eta, of point p relative to this
  // triangle. As a triple barycentric coordinates would be a multiple of
  // any vector normal to the plane determined by
  // v = (a.x - p.x, b.x - p.x, c.x -p.x) and w = (a.y - p.y, b.y - p.y, c.y -
  // p.y). Let \mu be the cross product of v & w, then point p is inside
  // triangle iff there is a \lambda \in \mathbb{R}^{*} (\lambda can be NOT
  // zero) s.t. \eta = \lambda & \mu. \eta is a unit 3D vector s.t. all of its
  // coordinates are non-negative ( lying on an edge counts ). Dividing \eta by
  // \lambda implies that it is enough to check that coordinates of \mu are all
  // either non-negative or non-positive. So,
  //                \mu = (1,1,0) & (-1,-1,0) are IN
  // but,
  //              \mu = (1,-1,0) & (-1, 1, 1) are OUT
  //
  //                  [   i          j              k   ]
  //       \mu = det  [ a.x - p.x  b.x - p.x   c.x -p.x ]
  //                  [ a.y - p.y  b.y - p.y   c.y -p.y ]

  std::array<Vertex::RingType, NUM_VERTICES_PER_FACE> mu = {
      (a->x - p->x) * (b->y - p->y) - (a->y - p->y) * (b->x - p->x),
      (a->y - p->y) * (c->x - p->x) - (a->x - p->x) * (c->y - p->y),
      (b->x - p->x) * (c->y - p->y) - (b->y - p->y) * (c->x - p->x)};

  // compare only nonzero entries of mu
  Vertex::RingType firstNonZeroElement, next;
  auto nextNonZeroElement = nonZeroElementsIterator<Vertex::RingType, NUM_VERTICES_PER_FACE>(mu);
  firstNonZeroElement     = nextNonZeroElement();

  while (!nextNonZeroElement.end()) {
    next = nextNonZeroElement();
    if (firstNonZeroElement > 0 != next > 0) return false;
  }

  return true;
}

Triangle::HalfEdgeRef Triangle::halfEdgeContainsPoint(const VertexRef p) {
  HalfEdgeRef cur = he;
  for (int i = 0; i < NUM_VERTICES_PER_FACE; i++) {
    if (cur->isVertexInHalfEdge(p)) return cur;
    cur = cur->getNext();
  }
  return nullptr;
}

Triangle::~Triangle() {
  // Triangle owns all the half edges of the face.
  delete he;
}
