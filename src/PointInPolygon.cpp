#include <include/PointInPolygon.h>
#include <include/SquareMatrix.h>

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
 * and the vector from point to edge.start.bool
 */
bool PointInPolygon::pointInPolygon(Point &p, RayType &ray_direction) {
  Intersection num     = None;
  int intersection_num = 0;
  RayType perturbed;

  for (auto &edge : boundary_curve) {
    num = edgeIntersect(p, ray_direction, edge);

    // Handle degenerate intersection by perturbing the angle of the ray.
    switch (num) {
      case Degenerate:
        perturbed = ray_direction.rotate(THETA);
        return pointInPolygon(p, perturbed);
      case OnEdge:
        return true;
      default:
        intersection_num += num;
        break;
    }
  }

  return (bool)(intersection_num % 2);
}

Intersection PointInPolygon::isRayInSector(RayType &a, RayType &b, RayType &ray) {
  // determine on what side of plane { a , b } ray is.
  double side               = det2D(a, b);
  std::array<double, 9> mat = {
      ray.x, ray.y, ray.length(),  //
      a.x,   a.y,   a.length(),    //
      b.x,   b.y,   b.length(),    //
  };
  double determinant = SquareMatrix<3, double>(mat).det();

  // determinant == 0  iff ray is in the plane determined by (a, ||a||) and
  // (b, ||b||). Furthermore, the ray emanating from point p intersects
  // edge.end or edge.start. While the model used here assumes that edges are
  // are half open, an end of an edge is the start of another edge, so
  // in either case the intersection inconclusive.
  if (determinant == 0) return Degenerate;

  // condition is dependent on whether normal to the plane spanned by
  // (a.x, a.y, a.length) & (b.x, b.y, b.length) points into +z/-z half of R^3.
  // side can't be 0, because collinearity is handled earlier.
  double coeff = (side > 0) ? determinant : -1 * determinant;
  return (coeff < 0) ? Proper : None;
}

Intersection PointInPolygon::edgeIntersect(Point &p, RayType &ray, Edge &edge) {
  // eagerly check for collinearity
  // cast Vector2D<int> to Vector2D<double>
  RayType point_to_end   = RayType(edge.end - p);
  RayType point_to_start = RayType(edge.start - p);

  // There are essentially two cases to handle if p, edge.start, and edge.end
  // are collinear.
  // 1. p is on either side of the edge.
  //   * if ray crosses the edge it intersects the whole edge
  //.    which is DEGENERATE
  //.  * if ray doesn't intersection return None
  // 2. if the point in the edge simply return OnEdge.
  if (det2D(point_to_start, point_to_end) == 0) {
    // p lies on edge including possibly the endpoints.
    if (point_to_start.dot(point_to_end) <= 0) return OnEdge;

    // At this point we know that the edge lies either completely to the
    // left or the right of point p. Does ray point in the direction
    // of and into the edge?
    if (det2D(ray, point_to_start) != 0 || ray.dot(point_to_start) < 0) return None;

    // only possibility left is that the ray emanating from point p intersects
    // the edge completely. Which is inconclusive.
    return Degenerate;
  }

  return isRayInSector(point_to_start, point_to_end, ray);
}

PointInPolygon::~PointInPolygon() {}
