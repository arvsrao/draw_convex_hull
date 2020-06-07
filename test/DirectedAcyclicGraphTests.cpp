#include <gtest/gtest.h>
#include <include/DirectedAcyclicNode.h>
#include <include/HalfEdge.h>
#include <include/Triangle.h>

using Point        = Vector2D<int>;
using HalfEdgeRef  = HalfEdge<int> *;
using TriangleType = Triangle<int>;

std::array<Point, 3> nodes = {Point(-4, -4), Point(0, 4), Point(4, 0)};
TriangleType triangle      = TriangleType(&nodes[0], &nodes[1], &nodes[2]);

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

// build a half edge description of a small triangulation
TEST(DAG, TriangulationRepresentationTest) {
  std::array<Point *, 4> vertices = {new Point(-5, 0), new Point(0, 10), new Point(5, 0),
                                     new Point(0, -10)};

  // left face
  auto ab = new HalfEdge<int>(vertices[0]);
  auto bd = new HalfEdge<int>(vertices[1], ab, nullptr, nullptr);
  ab->setNext(bd);
  auto da = new HalfEdge<int>(vertices[3], bd, ab, nullptr);
  bd->setNext(da);
  ab->setPrev(da);

  // right face
  auto bc = new HalfEdge<int>(vertices[1]);
  auto cd = new HalfEdge<int>(vertices[2], bc, nullptr, nullptr);
  bc->setNext(cd);
  auto db = new HalfEdge<int>(vertices[3], cd, bc, bd);
  cd->setNext(db);
  bc->setPrev(db);
  bd->setTwin(db);

  // (-1,0) is in the left face
  std::array<TriangleType, 2> faces = {TriangleType(ab), TriangleType(bc)};
  auto query_point                  = Point(-1, 0);

  ASSERT_TRUE(faces[0].containsPoint(query_point));
  ASSERT_FALSE(faces[1].containsPoint(query_point));

  // delete triangulation. delete the faces, which own the half edges, then the underlying vertices.
  for (auto &vertex : vertices) delete vertex;
}
