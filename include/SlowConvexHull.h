//
// Created by arvind on 20.03.20.
//

#ifndef DRAW_CONVEX_HULL_SLOWCONVEXHULL_H
#define DRAW_CONVEX_HULL_SLOWCONVEXHULL_H

#include <vector>
#include "Vector2D.h"
#include "Edge.h"

class SlowConvexHull {

 public:

  using Points = std::vector<Vector2D>;

  SlowConvexHull(const Points &points);
  virtual ~SlowConvexHull();

  Points getConvexHull();

 private:
  Points hull_points;

  inline bool isNegativeFrame(const Vector2D &p, const Vector2D &q);

  bool allOnRight(const Vector2D &p, const Vector2D &q, const Points &points);

  Points sortClockwise(std::vector<Edge> &segments);

};

#endif //DRAW_CONVEX_HULL_SLOWCONVEXHULL_H
