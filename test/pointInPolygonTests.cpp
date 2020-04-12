#include <gtest/gtest.h>
#include <include/PointInPolygon.h>

using Point = Vector2D<int>;

TEST(PointInPolygon, AngleMeasureTest) {
  Point ray = Point(1,0);
  Point end = Point(0,1);
  Point start = Point(-1,-1);

  std::cout << "\n\n";
  std::cout << ray.arccos() << " " << end.arccos() << " " << start.arccos() << "\n";
}

TEST(PointInPolygon, pointInPolygonTest) {

  std::vector<Point> polyLine = {Point(-4, -4), Point(0, 4), Point(4, 0)};
  std::vector<Edge> curve =
      {Edge(polyLine[0], polyLine[1]), Edge(polyLine[1], polyLine[2]), Edge(polyLine[2], polyLine[0])};

  PointInPolygon pipCalc = PointInPolygon(curve);
  Point query_point_inside = Point(0, 0);
  Point query_point_outside = Point(2, 3);

  ASSERT_FALSE(pipCalc.pointInPolygon(query_point_outside));
  ASSERT_TRUE(pipCalc.pointInPolygon(query_point_inside));

}
