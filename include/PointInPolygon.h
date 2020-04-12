#ifndef DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
#define DRAW_CONVEX_HULL_POINT_IN_POLYGON_H

#include "Edge.h"
#include <vector>

using PolyLine = std::vector<Edge>;

class PointInPolygon {

 public:
  PolyLine boundary_curve;

  PointInPolygon(const PolyLine &curve);

  bool pointInPolygon(Point &point);

  virtual ~PointInPolygon();

 private:
  int edgeIntersect(Point &point, Vector2D<double> &ray_direction, Edge &edge);

  bool pointInPolygon(Point &point, Vector2D<double> &ray_direction);
};

#endif //DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
