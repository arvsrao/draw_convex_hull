#include <gtest/gtest.h>
#include <include/PointInPolygon.h>

using Point   = Vector2D<int>;
using RayType = Vector2D<double>;

TEST(PointInPolygon, AngleMeasureTest) {
  Point ray   = Point(1, 0);
  Point end   = Point(0, 1);
  Point start = Point(-1, -1);

  std::cout << "\n\n";
  std::cout << ray.arccos() << " " << end.arccos() << " " << start.arccos()
            << "\n";

  double num = -0.99014577103682988;
  std::cout << "acos(-0.99014577103682988) = " << std::acos(num);

  RayType ra  = RayType(1.0, 0.1);
  RayType vec = RayType(-98.0, -24.0);
  double val  = ra.dot(vec) / ra.length() / vec.length();
  std::cout << "\n\n";

  std::cout << "val is " << val << "\n";
  std::cout << "acos(val) = " << std::acos(val);
  std::cout << "\n\n";

  std::vector<Point> polyLine = {Point(-4, -4), Point(0, 4), Point(4, 0)};
  PointInPolygon pipCalc      = PointInPolygon(polyLine);

  // std::cout << pipCalc.angleBetween(ra, vec) << "\n";
}

std::vector<Point> triangle        = {Point(-4, -4), Point(0, 4), Point(4, 0)};
PointInPolygon pointInTriangleCalc = PointInPolygon(triangle);

TEST(PointInPolygon, TriangleTest) {
  Point query_point_inside  = Point(0, 0);
  Point query_point_outside = Point(2, 3);

  ASSERT_FALSE(pointInTriangleCalc.pointInPolygon(query_point_outside));
  ASSERT_TRUE(pointInTriangleCalc.pointInPolygon(query_point_inside));

  // vertices of triangle should be considered inside triangle
  for (auto &vertex : triangle) {
    ASSERT_TRUE(pointInTriangleCalc.pointInPolygon(vertex));
  }

  // check that non-vertex points on the boundary are detected
  Point point_in_edge = Point((triangle[1].x + triangle[2].x) / 2,
                              (triangle[1].y + triangle[2].y) / 2);
  RayType ray         = RayType(1, 0);

  Intersection crossing_type = pointInTriangleCalc.edgeIntersect(
      point_in_edge, ray, pointInTriangleCalc.boundary_curve[1]);

  ASSERT_TRUE(crossing_type == OnEdge);
}

TEST(PointInPolygon, Collinearity) {
  // Check off-edge Collinearity situations
  Point left_of_edge   = Point(-4, 8);
  Point right_of_edge  = Point(8, -4);
  RayType ray_off_edge = RayType(1, 0);
  RayType ray_in_edge  = RayType(1, -1);

  Intersection left_crossing_type = pointInTriangleCalc.edgeIntersect(
      left_of_edge, ray_off_edge, pointInTriangleCalc.boundary_curve[1]);
  Intersection right_crossing_type = pointInTriangleCalc.edgeIntersect(
      right_of_edge, ray_off_edge, pointInTriangleCalc.boundary_curve[1]);

  ASSERT_TRUE(left_crossing_type == None);
  ASSERT_TRUE(right_crossing_type == None);

  left_crossing_type = pointInTriangleCalc.edgeIntersect(
      left_of_edge, ray_in_edge, pointInTriangleCalc.boundary_curve[1]);
  right_crossing_type = pointInTriangleCalc.edgeIntersect(
      right_of_edge, ray_in_edge, pointInTriangleCalc.boundary_curve[1]);

  ASSERT_TRUE(left_crossing_type == Degenerate);
  ASSERT_TRUE(right_crossing_type == None);
}

TEST(PointInPolygon, isRayInSectorTest) {
  // test isRayInSector function
  RayType a   = RayType(-0.70046, -0.71368);
  RayType b   = RayType(-0.406138, 0.913811);
  RayType ray = RayType(1.0, 0.0);

  ASSERT_FALSE(PointInPolygon::isRayInSector(a, b, ray));
}

// use the same shape and PointInPolygon calculator in following tests.
std::vector<Point> polyLine = {
    Point(-44, -134), Point(-67, -112), Point(-105, -39), Point(-64, 42),
    Point(5, 96),     Point(93, 47),    Point(121, -15),  Point(99, -83),
    Point(85, -106),  Point(61, -60),   Point(63, -14),   Point(34, 40),
    Point(-14, 22),   Point(-46, -33),  Point(-22, -65),  Point(-33, -107)};

PointInPolygon pipCalc = PointInPolygon(polyLine);

TEST(PointInPolygon, PointsNotInShape) {
  std::vector<Point> pointsNotInShape = {Point(-7, -15), Point(-3, -14),
                                         Point(14, -14), Point(17, -15)};

  for (auto &wp : pointsNotInShape) {
    ASSERT_FALSE(pipCalc.pointInPolygon(wp));
  }
}

TEST(PointInPolygon, BoundaryOfShape) {
  for (auto &wp : polyLine) {
    EXPECT_TRUE(pipCalc.pointInPolygon(wp));
  }
}
