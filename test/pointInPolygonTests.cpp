#include <gtest/gtest.h>
#include <include/PointInPolygon.h>

using Point = Vector2D<int>;

TEST(PointInPolygon, AngleMeasureTest) {
  Point ray = Point(1, 0);
  Point end = Point(0, 1);
  Point start = Point(-1, -1);

  std::cout << "\n\n";
  std::cout << ray.arccos() << " " << end.arccos() << " " << start.arccos() << "\n";

  double num = -0.99014577103682988;
  std::cout << "acos(-0.99014577103682988) = " << std::acos(num);

  Vector2D<double> ra = Vector2D<double>(1.0, 0.1);
  Vector2D<double> vec = Vector2D<double>(-98.0, -24.0);
  double val = ra.dot(vec) / ra.length() / vec.length();\
    std::cout << "\n\n";

  std::cout << "val is " << val << "\n";
  std::cout << "acos(val) = " << std::acos(val);

  std::cout << "\n\n";

  std::vector<Point> polyLine = {Point(-4, -4), Point(0, 4), Point(4, 0)};
  PointInPolygon pipCalc = PointInPolygon(polyLine);

  std::cout << pipCalc.angleBetween(ra, vec) << "\n";
}

TEST(PointInPolygon, TriangleTest) {

  std::vector<Point> polyLine = {Point(-4, -4), Point(0, 4), Point(4, 0)};
  PointInPolygon pipCalc = PointInPolygon(polyLine);

  Point query_point_inside = Point(0, 0);
  Point query_point_outside = Point(2, 3);

  ASSERT_FALSE(pipCalc.pointInPolygon(query_point_outside));
  ASSERT_TRUE(pipCalc.pointInPolygon(query_point_inside));
}

TEST(PointInPolygon, PointsNotInShape) {

  std::vector<Point> pointsNotInShape = {Point(-7, -15),
                                         Point(-3, -14),
                                         Point(14, -14),
                                         Point(17, -15)};

  std::vector<Point> polyLine = {
      Point(-44, -134),
      Point(-67, -112),
      Point(-105, -39),
      Point(-64, 42),
      Point(5, 96),
      Point(93, 47),
      Point(121, -15),
      Point(99, -83),
      Point(85, -106),
      Point(61, -60),
      Point(63, -14),
      Point(34, 40),
      Point(-14, 22),
      Point(-46, -33),
      Point(-22, -65),
      Point(-33, -107)
  };

  PointInPolygon pipCalc = PointInPolygon(polyLine);

  for (auto &wp : pointsNotInShape) {
    ASSERT_FALSE(pipCalc.pointInPolygon(wp));
  }
}
