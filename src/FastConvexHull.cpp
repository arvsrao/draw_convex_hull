#include <algorithm>
#include <vector>
#include <include/FastConvexHull.h>

Points &FastConvexHull::getConvexHull() {
  return hull_points;
}

/*
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
bool FastConvexHull::isNegativeFrame(const Point &p, const Point &q) const {
  return (p.x * q.y - q.x * p.y) < 0;
}

// do three points make a right turn.
// same as asking if 'end' lies to the right of
// segment start-middle
bool FastConvexHull::rightTurn(const Point &start, const Point &middle, const Point &end) const {
  return isNegativeFrame(middle - start, end - start);
}

Points FastConvexHull::_convexHull(const Points &points) {
  Points _hull;

  for (auto it = points.begin(); it != points.end(); it++) {
    _hull.push_back(*it);
    while (_hull.size() > 2) {
      auto c = _hull.back();
      _hull.pop_back();
      auto b = _hull.back();
      _hull.pop_back();
      auto a = _hull.back();
      if (!rightTurn(a, b, c)) _hull.push_back(c);
      else {
        _hull.push_back(b);
        _hull.push_back(c);
        break;
      }
    }
  }
  return _hull;
}

FastConvexHull::FastConvexHull(Points &points) {
  std::sort(points.begin(), points.end(), Point::compareVector2D);
  hull_points = _convexHull(points);

  std::sort(points.begin(),
            points.end(),
            [&](Point &a, Point &b) { return (a.x == b.x) ? a.y > b.y : a.x > b.x; });
  auto bottom_hull = _convexHull(points);

  // concatenate top and bottom hulls.
  hull_points.pop_back();
  hull_points.insert(hull_points.end(), bottom_hull.begin(), bottom_hull.end() - 1);
}

FastConvexHull::~FastConvexHull() {}

