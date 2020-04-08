#ifndef DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
#define DRAW_CONVEX_HULL_POINT_IN_POLYGON_H

#include "Edge.h"

using PolyLine = std::vector<Edge>;

class PointInPolygon {

public:
  PolyLine boundary_curve;

  PointInPolygon(PolyLine &curve);

  bool pointInPolygon(Vector2D &point);

private:
  int PointInPolygon::edgeIntersect(Vector2D &point, Vector2D &ray_direction, Edge &edge);

  virtual ~PointInPolygon();
};

#endif //DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
