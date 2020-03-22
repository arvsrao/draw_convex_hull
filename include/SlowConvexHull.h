//
// Created by arvind on 20.03.20.
//

#ifndef DRAW_CONVEX_HULL_SLOWCONVEXHULL_H
#define DRAW_CONVEX_HULL_SLOWCONVEXHULL_H

#include "Edge.h"
#include "ConvexHull.h"

class SlowConvexHull : public ConvexHull {

 public:

  SlowConvexHull(const Points &points);
  virtual ~SlowConvexHull();

  Points &getConvexHull();

 private:
  Points hull_points;

  inline bool isNegativeFrame(const Vector2D &p, const Vector2D &q);

  bool allOnRight(const Vector2D &p, const Vector2D &q, const Points &points);

  Points sortClockwise(std::vector<Edge> &segments);

};

#endif //DRAW_CONVEX_HULL_SLOWCONVEXHULL_H
