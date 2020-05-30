#include <array>

template <typename T>
Triangle<T>::Triangle(PointType _a, PointType _b, PointType _c)
    : a(_a), b(_b), c(_c) {}

template <typename T, unsigned N>
class nonZeroElementsIterator {
 public:
  std::array<T, N> mu;

  // the index of the most recent encountered nonzero element of mu.
  int currentIdx;

  // the largest index of all nonzero entries of mu
  int endIdx;

  nonZeroElementsIterator(std::array<T, N>& _mu)
      : mu(_mu), currentIdx(-1), endIdx(N - 1) {
    while (!mu[endIdx]) --endIdx;
  }

  // has iterator reached the last nonzero element
  bool end() { return currentIdx >= endIdx; }

  T operator()() {
    if (end()) return mu[endIdx];
    currentIdx++;
    while (!mu[currentIdx]) currentIdx++;
    return mu[currentIdx];
  }
};

template <typename T>
bool Triangle<T>::containsPoint(PointType& p) {
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

  const unsigned M    = 3;
  std::array<T, M> mu = {(a.x - p.x) * (b.y - p.y) - (a.y - p.y) * (b.x - p.x),
                         (a.y - p.y) * (c.x - p.x) - (a.x - p.x) * (c.y - p.y),
                         (b.x - p.x) * (c.y - p.y) - (b.y - p.y) * (c.x - p.x)};

  // compare only nonzero entries of mu
  auto nextNonZeroElement = nonZeroElementsIterator<T, M>(mu);
  T curr                  = nextNonZeroElement();
  T next;

  while (!nextNonZeroElement.end()) {
    next = nextNonZeroElement();
    if (curr > 0 != next > 0) return false;
    curr = next;
  }

  return true;
}

template <typename T>
Triangle<T>::~Triangle() {}
