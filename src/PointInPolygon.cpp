#define DEGENERATE -2

#include <algorithm>    // std::max
#include <include/PointInPolygon.h>

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

  int num = 0;
  int intersection_num = 0;

  for (auto &edge : boundary_curve) {

    num = edgeIntersect(point, ray_direction, edge);

    // Handle degenerate intersection by perturbing the ray by some epsilon.
    if (num == DEGENERATE) {
      RayType perturbed_direction = ray_direction + RayType(0.0, EPSILON);
      return pointInPolygon(point, perturbed_direction);
    }

    intersection_num += num;
  }

  return (bool) (intersection_num % 2);
}

int determinant(const RayType &p, const RayType &q) {
  double retVal = (p.x * q.y - q.x * p.y);

  if (retVal > 0) return +1;
  if (retVal < 0) return -1;
  return 0;
}

bool isRayInSector(RayType &a, RayType &b, RayType &ray) {

  // compute normal of start_vec && end_vec.
  // correct normal to always point into positive half of R^3
  //
  //      [ i    j    k ]
  //  det [ a.x  a.y  1 ]
  //      [ b.x  b.y  1 ]	
  double eta_x = a.y - b.y;	
  double eta_y = b.x - a.x;	
  double eta_z = std::abs(a.x * b.y - b.x * a.y);	

  // eta * ray
  return eta_x * ray.x + eta_y * ray.y + eta_z <= 0 
}

int PointInPolygon::edgeIntersect(Point &point, RayType &ray_direction, Edge &edge) {

  /** Imagine an infinite length ray from point in direction (1, EPSILON),
  //    \beta(t) = t * <1, EPSILON> + point
  // Then determine if the ray \beta(t) intersects @param edge by
  // checking if the ray lies in between vectors from point to edge.end
  // and the vector from point to edge.start.
  **/

  // cast Vector2D<int> to Vector2D<double)
  RayType end_vec = RayType(edge.end - point);
  RayType start_vec = RayType(edge.start - point);

  // Edges are considered half open interval like [start, end), otherwise
  // intersections at segment endpoints will be counted twice.
  int ray_placement = determinant(ray_direction, end_vec);
  if (ray_placement == 0 && end_vec.dot(ray_direction) > 0) return 0; // ray intersects {{edge.end}}

  // ray intersects {{edge.start}} in a non-generic way;
  // which means determining intersection is inconclusive when querying
  // in the direction of ray_direction. Additionally, the degenerate case
  // of whole edge intersection is also handled here, because the whole edge
  // includes {{edge.start}}.
  if (determinant(ray_direction, start_vec) == 0 && start_vec.dot(ray_direction) > 0) return DEGENERATE;

  ray_placement += determinant(ray_direction, start_vec);

  // check if horizontal line that goes through ray_direction crosses the edge.
  // 
  if (ray_placement < -1 || ray_placement > 1) return 0;

  // embed start_vec && end_vec && ray_direction into z=1
  // plane of R^3. compute normal of start_vec && end_vec.
  // correct normal to always point into positive half of R^3
  return (int) isRayInSector(start_vec, end_vec, ray_direction);
}

PointInPolygon::~PointInPolygon() {}