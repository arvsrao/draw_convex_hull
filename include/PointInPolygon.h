#ifndef DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
#define DRAW_CONVEX_HULL_POINT_IN_POLYGON_H

#include "Edge.h"

using PolyLine = std::vector<Edge>;

class PointInPolygon {

public:
  PolyLine boundary_curve;

  PointInPolygon(PolyLine &curve);

  bool pointInPolygon(Point &point);

private:
  int PointInPolygon::edgeIntersect(Vector2D &point, Vector2D &ray_direction, Edge &edge);

  bool pointInPolygon(Point &point, Vector2D<double> &ray_direction);

  virtual ~PointInPolygon();
};

#endif //DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
