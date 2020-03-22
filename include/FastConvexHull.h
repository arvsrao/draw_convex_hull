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

  inline bool isNegativeFrame(const Vector2D &p, const Vector2D &q) const;

  Points _convexHull(const Points &points);

  inline bool rightTurn(const Vector2D &start, const Vector2D &middle, const Vector2D &end) const;

};

#endif //DRAW_CONVEX_HULL_FASTCONVEXHULL_H
