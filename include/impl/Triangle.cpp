#include <array>

template <typename T>
Triangle<T>::Triangle(PointType _a, PointType _b, PointType _c)
    : a(_a), b(_b), c(_c) {}

template <typename T, unsigned N>
class nextNonZeroIndexClosure {
 public:
  std::array<T, N> mu;
  nextNonZeroIndexClosure(std::array<T, N>& _mu) : mu(_mu) {}

  unsigned operator()(unsigned idx) {
    while (!mu[idx]) idx++;
    return idx;
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

  // ignore zero coordinates. when encountered increment index
  auto nextNonZeroIndex = nextNonZeroIndexClosure<T, M>(mu);
  unsigned idx1         = nextNonZeroIndex(0);
  unsigned idx2         = nextNonZeroIndex(idx1 + 1);

  while (idx1 < M && idx2 < M) {
    if (mu[idx1] > 0 != mu[idx2] > 0) return false;
    idx1 = idx2;
    idx2 = nextNonZeroIndex(idx1 + 1);
  }
  return true;
}

template <typename T>
Triangle<T>::~Triangle() {}
