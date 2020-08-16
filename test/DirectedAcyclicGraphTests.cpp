#include <DelaunayTriangulator.h>
#include <TriangleWithOneSymbolicPoint.h>
#include <gtest/gtest.h>
#include <include/DirectedAcyclicNode.h>
#include <include/HalfEdge.h>
#include <include/Triangle.h>
#include <include/Vertex.h>

using HalfEdgeRef = HalfEdge *;

std::array<Vertex, 3> nodes = {Vertex(-4, -4), Vertex(0, 4), Vertex(4, 0)};
Triangle triangle           = Triangle(&nodes[0], &nodes[1], &nodes[2]);

TEST(DAG, CreationTest) {
  auto dag = new DirectedAcyclicNode<Triangle>(&triangle);

  ASSERT_EQ(dag->getFace()->a, triangle.a);
  ASSERT_EQ(dag->getFace()->b, triangle.b);
  ASSERT_EQ(dag->getFace()->c, triangle.c);

  ASSERT_FALSE(dag->hasChildren());
}

TEST(DAG, PointInTriangleTest) {
  Vertex inside_point  = Vertex(0.0, 0.0);
  Vertex outside_point = Vertex(4, 4);
  Vertex edge_point_ab = Vertex(-2, 0);  // on edge {ab}
  Vertex edge_point_bc = Vertex(2, 2);   // on edge {bc}
  Vertex edge_point_ac = Vertex(0, 2);   // on edge {ac}

  ASSERT_TRUE(triangle.containsPoint(inside_point));
  ASSERT_FALSE(triangle.containsPoint(outside_point));

  ASSERT_TRUE(triangle.containsPoint(edge_point_ab));
  ASSERT_TRUE(triangle.containsPoint(edge_point_bc));
  ASSERT_TRUE(triangle.containsPoint(edge_point_ac));
}

// build a half edge description of a small triangulation
TEST(DAG, TriangulationRepresentationTest) {
  std::array<Vertex *, 4> vertices = {new Vertex(-5.0, 0.0), new Vertex(0.0, 10.0),
                                      new Vertex(5.0, 0.0), new Vertex(0.0, -10.0)};

  // left face
  auto ab = new HalfEdge(vertices[0]);
  auto bd = new HalfEdge(vertices[1], ab, nullptr, nullptr);
  ab->setNext(bd);
  auto da = new HalfEdge(vertices[3], bd, ab, nullptr);
  bd->setNext(da);
  ab->setPrev(da);

  // right face
  auto bc = new HalfEdge(vertices[1]);
  auto cd = new HalfEdge(vertices[2], bc, nullptr, nullptr);
  bc->setNext(cd);
  auto db = new HalfEdge(vertices[3], cd, bc, bd);
  cd->setNext(db);
  bc->setPrev(db);
  bd->setTwin(db);

  // (-1,0) is in the left face
  std::array<Triangle *, 2> faces = {new Triangle(ab), new Triangle(bc)};
  auto query_point                = Vertex(-1, 0);

  ASSERT_TRUE(faces[0]->containsPoint(query_point));
  ASSERT_FALSE(faces[1]->containsPoint(query_point));

  // delete triangulation. delete the faces, which own the half edges, then the underlying vertices.
  for (auto &face : faces) delete face;
  for (auto &vertex : vertices) delete vertex;
}

TEST(DAG, SymbolicPointsTest) {
  auto left_symbol_point  = Vertex(Vertex::Symbol::Left);
  auto right_symbol_point = Vertex(Vertex::Symbol::Right);

  auto origin                      = Vertex(0.0, 0.0);
  auto end                         = Vertex(0.0, 10.0);
  auto point_inside_left_triangle  = Vertex(-10.0, 0.0);
  auto point_inside_right_triangle = Vertex(10.0, 0.0);

  Triangle *triangle_with_left_symbolic_point =
      new TriangleWithOneSymbolicPoint(&left_symbol_point, &origin, &end);

  Triangle *triangle_with_right_symbolic_point =
      new TriangleWithOneSymbolicPoint(&right_symbol_point, &origin, &end);

  ASSERT_TRUE(triangle_with_left_symbolic_point->containsPoint(point_inside_left_triangle));
  ASSERT_TRUE(triangle_with_right_symbolic_point->containsPoint(point_inside_right_triangle));
  ASSERT_FALSE(triangle_with_right_symbolic_point->containsPoint(point_inside_left_triangle));

  ASSERT_FALSE(left_symbol_point == end);
  ASSERT_TRUE(left_symbol_point < end);
  ASSERT_FALSE(right_symbol_point < end);
}