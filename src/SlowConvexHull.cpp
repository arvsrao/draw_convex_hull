
#include <algorithm>
#include <include/SlowConvexHull.h>

Points &SlowConvexHull::getConvexHull() {
  return hull_points;
}

/*
*
* @param p: is a fixed orient segment under consideration
* when return is false, @param q is to the left of @param p
*
*  det [p.x p.y]
*      [q.x q.y]
*
*  OR
*
*  the right _|_ vector of p is (p.y, -p.x)
*
*  (q.x,q.y) * (p.y, -p.x) = q.x * p.y - q.y * p.x > 0
*
*/
inline bool SlowConvexHull::isNegativeFrame(const Point &p, const Point &q) {
  return (p.x * q.y - q.x * p.y) < 0;
}

bool SlowConvexHull::allOnRight(const Point &p, const Point &q, const Points &points) {
  for (size_t i = 0; i < points.size(); i++) {
    auto v = points[i];
    if (q == v || p == v) continue;
    if (!isNegativeFrame(q - p, v - p)) return false;
  }
  return true;
}

Points SlowConvexHull::sortClockwise(std::vector<Edge> &segments) {
  std::sort(segments.begin(), segments.end(), Edge::segmentCompare);
  Points pts;
  for (unsigned long i = 0; i < segments.size(); i++) pts.push_back(segments[i].start);

  // remove duplicates.
//    auto last = unique(pts.begin(), pts.end());
//    pts.erase(last, pts.end());
  return pts;
}

SlowConvexHull::SlowConvexHull(const Points &points) {
  std::vector<Edge> hull;

  for (auto p = points.begin(); p != points.end(); p++) {
    for (auto q = points.begin(); q != points.end(); q++) {
      if (*p == *q) continue;
      if (allOnRight(*p, *q, points)) {
        hull.push_back(Edge(*p, *q));
      }
    }
  }

  hull_points = sortClockwise(hull);
}

SlowConvexHull::~SlowConvexHull() {}