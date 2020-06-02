#include <gtest/gtest.h>
#include <include/DirectedAcyclicNode.h>
#include <include/Triangle.h>

using Point        = Vector2D<int>;
using TriangleType = Triangle<int>;

TriangleType triangle = TriangleType(Point(-4, -4), Point(0, 4), Point(4, 0));

TEST(DAG, CreationTest) {
  auto dag           = new DirectedAcyclicNode<TriangleType>(triangle);
  auto expected_data = dag->getData();

  ASSERT_EQ(expected_data.a, triangle.a);
  ASSERT_EQ(expected_data.b, triangle.b);
  ASSERT_EQ(expected_data.c, triangle.c);

  ASSERT_TRUE(dag->getChildren().empty());
}

TEST(DAG, PointInTriangleTest) {
  Point inside_point  = Point(0, 0);
  Point outside_point = Point(4, 4);
  Point edge_point_ab = Point(-2, 0);  // on edge {ab}
  Point edge_point_bc = Point(2, 2);   // on edge {bc}
  Point edge_point_ac = Point(0, 2);   // on edge {ac}

  ASSERT_TRUE(triangle.containsPoint(inside_point));
  ASSERT_FALSE(triangle.containsPoint(outside_point));

  ASSERT_TRUE(triangle.containsPoint(edge_point_ab));
  ASSERT_TRUE(triangle.containsPoint(edge_point_bc));
  ASSERT_TRUE(triangle.containsPoint(edge_point_ac));
}
