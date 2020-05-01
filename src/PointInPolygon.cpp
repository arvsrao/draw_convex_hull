#define DEGENERATE -2

#include <include/PointInPolygon.h>

#include <algorithm>  // std::max

// a convenient class internal type
using RayType = Vector2D<double>;

// rotation angle for handling degeneracies
const double THETA = PI / 20.0;

PointInPolygon::PointInPolygon(const PolyLine &curve) : boundary_curve(curve) {}

PointInPolygon::PointInPolygon(const std::vector<Point> &curve) {
  for (int i = 0; i < curve.size(); i++) {
    boundary_curve.push_back(Edge(curve[i], curve[(i + 1) % curve.size()]));
  }
}

/*
 * Compute oriented intersection number \mod 2
 * relative to {{point}}.
 */
bool PointInPolygon::pointInPolygon(Point &point) {
  RayType horizontal_ray = RayType(1.0, 0.0);
  return pointInPolygon(point, horizontal_ray);
}

/**
 * Imagine an infinite length ray from point p in direction (1, 0),
 *    \r(t) = t * <1, 0> + p
 * Then determine if the ray \r(t) intersects an edge by
 * checking if the ray lies in between vectors from point to edge.end
 */
and the vector from point to edge.start.bool PointInPolygon::pointInPolygon(
    Point &p, RayType &ray_direction) {
  int num              = 0;
  int intersection_num = 0;

  for (auto &edge : boundary_curve) {
    num = edgeIntersect(p, ray_direction, edge);

    // Handle degenerate intersection by perturbing the angle of the ray.
    if (num == DEGENERATE) {
      RayType perturbed = ray_direction.rotate(THETA);
      return pointInPolygon(p, perturbed);
    }

    intersection_num += num;
  }

  return (bool)(intersection_num % 2);
}

int PointInPolygon::isRayInSector(RayType &a, RayType &b, RayType &ray) {
  // compute normal of a && b.
  //
  //      [ r.x  r.y  r.length ]       [ b.length a.length r.length ]
  //  det [ a.x  a.y  a.length ] = det [ b.y      a.y      r.y      ]
  //      [ b.x  b.y  b.length ]       [ b.x      a.x      r.x      ]
  double side        = det2D(a, b);
  double determinant = b.length() * det2D(ray, a) + a.length() * det2D(b, ray) +
                       ray.length() * side;

  // determinat == 0 implies the ray emmenating from point p intersects
  // edge.end or edge.start. While the model use here assumes that edges are
  // are half open, an end of an edge is the start of another edge, so
  // in either case the intersection inconclusive.
  if (determinant == 0) return DEGENERATE;

  // condition is dependent on whether normal to the plane spanned by
  // (a.x, a.y, a.length) & (b.x, b.y, b.length) points into +z/-z half of R^3.
  // side can't be 0, because collinearity is handled earlier.
  double coeff = (side > 0) ? determinant : -1 * determinant;
  return (coeff < 0) ? 1 : 0;
}

int PointInPolygon::edgeIntersect(Point &p, RayType &ray, Edge &edge) {
  // eagarly check for colinearity
  // cast Vector2D<int> to Vector2D<double>
  RayType point_to_end   = RayType(edge.end - p);
  RayType point_to_start = RayType(edge.start - p);

  // There 3 collinearity cases
  // * before or after the edge
  //   * if ray crosses the edge it intersects the whole edge
  //.    which is DEGENERATE
  //.  * if ray doesn't intersection return 0
  // * if the point in the edge simply return true.
  if (det2D(point_to_start, point_to_end) == 0) {
    // p lies on edge include possibly the endpoints.
    if (point_to_start.dot(point_to_end) <= 0)
      return 0

          // At this point we know that the edge lies either completely to the
          // left or the right of point p.
          if (det2D(ray, point_to_start) != 0 ||
              ray.dot(point_to_start) < 0) return 0;

    // only possiblity left is that that ray emmenating from point p intersects
    // the edge completely. Which is inconclusive.
    return DEGENERATE;
  }

  // check if horizontal line that goes through ray_direction crosses the edge.
  if ((det2D(point_to_end, ray) < 0) == (det2D(point_to_start, ray) < 0))
    return 0;

  // embed start_vec && end_vec && ray_direction into S^1 in the Z=1
  // plane of R^3. Compute the normal of embedded start_vec && end_vec.
  // Then compare the normal to ray_direction.
  return isRayInSector(point_to_start, point_to_end, ray);
}

PointInPolygon::~PointInPolygon() {}
