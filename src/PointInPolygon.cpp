#define DEGENERATE -2

#include <include/PointInPolygon.h>

#include <algorithm>  // std::max

// a convenient class internal type
using RayType = Vector2D<double>;

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

bool PointInPolygon::pointInPolygon(Point &point, RayType &ray_direction) {
  const double EPSILON = 0.1;

  int num              = 0;
  int intersection_num = 0;
  ray_direction.normalize();  // project onto S^1

  for (auto &edge : boundary_curve) {
    num = edgeIntersect(point, ray_direction, edge);

    // Handle degenerate intersection by perturbing the ray by some epsilon.
    if (num == DEGENERATE) {
      RayType perturbed_direction = ray_direction + RayType(0.0, EPSILON);
      return pointInPolygon(point, perturbed_direction);
    }

    intersection_num += num;
  }

  return (bool)(intersection_num % 2);
}

double det2D(const RayType &p, const RayType &q) {
  return p.x * q.y - q.x * p.y;
}

bool PointInPolygon::isRayInSector(RayType &a, RayType &b, RayType &ray) {
  // compute normal of start_vec && end_vec.
  //
  //      [ r.x  r.y  r.length ]       [ b.length a.length r.length ]
  //  det [ a.x  a.y  a.length ] = det [ b.y      a.y      r.y      ]
  //      [ b.x  b.y  b.length ]       [ b.x      a.x      r.x      ]
  double side = det2D(a, b);
  double determinant =
      b.length() * det2D(ray, a) + a.length() * det2D(b, ray) + side;

  // condition is dependent on whether normal to the plane spanned by
  // (a.x, a.y, a.length) & (b.x, b.y, b.length) points into +z/-z half of R^3.
  // Equality implies query point lies on the line between a and b. In the
  // context of edgeIntersect where this function is called, equality means
  // the query point lies on the given edge.
  return (side * determinant) <= 0;
}

int PointInPolygon::edgeIntersect(Point &point, RayType &ray_direction,
                                  Edge &edge) {
  /** Imagine an infinite length ray from point in direction (1, EPSILON),
  //    \beta(t) = t * <1, EPSILON> + point
  // Then determine if the ray \beta(t) intersects @param edge by
  // checking if the ray lies in between vectors from point to edge.end
  // and the vector from point to edge.start.
  **/

  // cast Vector2D<int> to Vector2D<double>
  RayType end_vec   = RayType(edge.end - point);
  RayType start_vec = RayType(edge.start - point);

  // ray intersects {{edge.start}} in a non-generic way;
  // which means determining intersection is inconclusive when querying
  // in the direction of ray_direction. Additionally, the degenerate case
  // of whole edge intersection is also handled here, because the whole edge
  // includes {{edge.start}}.
  if (det2D(start_vec, ray_direction) == 0 && start_vec.dot(ray_direction) >= 0)
    return DEGENERATE;

  // Edges are considered half open intervals like [start, end), otherwise
  // intersections at segment endpoints will be counted twice. Therefore,
  // if ray_direction intersects {{edge.end}} => no crossing.
  if (det2D(end_vec, ray_direction) == 0 && end_vec.dot(ray_direction) >= 0)
    return 0;

  // check if horizontal line that goes through ray_direction crosses the edge.
  if (det2D(ray_direction, end_vec) < 0 == det2D(ray_direction, start_vec) < 0)
    return 0;

  // embed start_vec && end_vec && ray_direction into S^1 in the Z=1
  // plane of R^3. Compute the normal of embedded start_vec && end_vec.
  // Then compare the normal to ray_direction.
  return (int)isRayInSector(start_vec, end_vec, ray_direction);
}

PointInPolygon::~PointInPolygon() {}
