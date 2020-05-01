#ifndef DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
#define DRAW_CONVEX_HULL_POINT_IN_POLYGON_H

#include <vector>

#include "Edge.h"

// convenient internal type aliases
using RayType  = Vector2D<double>;
using PolyLine = std::vector<Edge>;

class PointInPolygon {
 public:
  PolyLine boundary_curve;

  PointInPolygon(const PolyLine &curve);

  PointInPolygon(const std::vector<Point> &curve);

  bool pointInPolygon(Point &point);

  static bool isRayInSector(RayType &a, RayType &b, RayType &ray);

  virtual ~PointInPolygon();

 private:
  double angleBetween(const RayType &p, const RayType &q) {
    return std::acos(p.dot(q) / (p.length() * q.length()));
  }

  static double det2D(const RayType &p, const RayType &q) {
    return p.x * q.y - q.x * p.y;
  }

  int edgeIntersect(RayType &ray, RayType &point_to_start,
                    RayType &point_to_end);

  bool pointInPolygon(Point &point, RayType &ray_direction);
};

#endif  // DRAW_CONVEX_HULL_POINT_IN_POLYGON_H
