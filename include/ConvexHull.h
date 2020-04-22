#ifndef DRAW_CONVEX_HULL_CONVEXHULL_H
#define DRAW_CONVEX_HULL_CONVEXHULL_H

#include <vector>

#include "Vector2D.h"

// Abstract Class representing a Convex Hull Calculator.

using Point  = Vector2D<int>;
using Points = std::vector<Point>;

class ConvexHull {
 public:
  virtual ~ConvexHull(){};

  virtual Points &getConvexHull() = 0;

 private:
  Points hull_points;
};

#endif  // DRAW_CONVEX_HULL_CONVEXHULL_H
