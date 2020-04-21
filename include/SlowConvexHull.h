//
// Created by arvind on 20.03.20.
//

#ifndef DRAW_CONVEX_HULL_SLOWCONVEXHULL_H
#define DRAW_CONVEX_HULL_SLOWCONVEXHULL_H

#include "ConvexHull.h"
#include "Edge.h"

class SlowConvexHull : public ConvexHull {
 public:
  SlowConvexHull(const Points &points);
  virtual ~SlowConvexHull();

  Points &getConvexHull();

 private:
  Points hull_points;

  inline bool isNegativeFrame(const Point &p, const Point &q);

  bool allOnRight(const Point &p, const Point &q, const Points &points);

  Points sortClockwise(std::vector<Edge> &segments);
};

#endif  // DRAW_CONVEX_HULL_SLOWCONVEXHULL_H
