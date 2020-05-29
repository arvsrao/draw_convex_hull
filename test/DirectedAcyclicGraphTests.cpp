#include <gtest/gtest.h>
#include <include/DirectedAcyclicNode.h>
#include <include/Triangle.h>

using Point        = Vector2D<int>;
using TriangleType = Triangle<int>;

TEST(DAG, CreationTest) {
  TriangleType triangle = TriangleType(Point(-4, -4), Point(0, 4), Point(4, 0));
  auto dag              = new DirectedAcyclicNode<TriangleType>(triangle);
  auto expected_data    = dag->getData();

  ASSERT_EQ(expected_data.a, triangle.a);
  ASSERT_EQ(expected_data.b, triangle.b);
  ASSERT_EQ(expected_data.c, triangle.c);

  ASSERT_TRUE(dag->getChildren().empty());
}