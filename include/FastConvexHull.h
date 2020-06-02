#ifndef DRAW_CONVEX_HULL_FASTCONVEXHULL_H
#define DRAW_CONVEX_HULL_FASTCONVEXHULL_H

#include "ConvexHull.h"

class FastConvexHull : public ConvexHull {
 public:
  FastConvexHull(Points &points);
  virtual ~FastConvexHull();

  Points &getConvexHull();

 private:
  Points hull_points;

  inline bool isNegativeFrame(const Point &p, const Point &q) const;

  Points _convexHull(const Points &points);

  inline bool rightTurn(const Point &start, const Point &middle, const Point &end) const;
};

#endif  // DRAW_CONVEX_HULL_FASTCONVEXHULL_H
