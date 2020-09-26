#include <TriangleWithOneSymbolicPoint.h>
#include <gtest/gtest.h>
#include <include/DelaunayTriangulator.h>
#include <include/HalfEdge.h>

using HalfEdgeRef = HalfEdge *;
using Vertex      = Vector2D<double>;

std::array<Vertex, 3> nodes = {Vertex(-4, -4), Vertex(0, 4), Vertex(4, 0)};
Triangle triangle           = Triangle(&nodes[0], &nodes[1], &nodes[2]);

TEST(DAG, PointInTriangleTest) {
  Vertex inside_point  = Vertex(0.0, 0.0);
  Vertex outside_point = Vertex(4, 4);
  Vertex edge_point_ab = Vertex(-2, 0);  // on edge {ab}
  Vertex edge_point_bc = Vertex(2, 2);   // on edge {bc}
  Vertex edge_point_ac = Vertex(0, 2);   // on edge {ac}

  ASSERT_TRUE(triangle.containsPoint(&inside_point));
  ASSERT_FALSE(triangle.containsPoint(&outside_point));

  ASSERT_TRUE(triangle.containsPoint(&edge_point_ab));
  ASSERT_TRUE(triangle.containsPoint(&edge_point_bc));
  ASSERT_TRUE(triangle.containsPoint(&edge_point_ac));
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

  ASSERT_TRUE(faces[0]->containsPoint(&query_point));
  ASSERT_FALSE(faces[1]->containsPoint(&query_point));

  // delete triangulation. delete the faces, which own the half edges, then the underlying vertices.
  for (auto &face : faces) delete face;
  for (auto &vertex : vertices) delete vertex;
}

TEST(DAG, SymbolicPointsTest) {
  auto origin                      = Vertex(0.0, 0.0);
  auto end                         = Vertex(5.0, 5.0);
  auto point_inside_left_triangle  = Vertex(-2.0, 4.0);
  auto point_inside_right_triangle = Vertex(6.0, 4.0);

  Triangle *triangle_with_left_symbolic_point =
      new TriangleWithOneSymbolicPoint(HalfEdge::Symbol::Left, &origin, &end);

  Triangle *triangle_with_right_symbolic_point =
      new TriangleWithOneSymbolicPoint(HalfEdge::Symbol::Right, &origin, &end);

  ASSERT_TRUE(triangle_with_left_symbolic_point->containsPoint(&point_inside_left_triangle));
  ASSERT_TRUE(triangle_with_right_symbolic_point->containsPoint(&point_inside_right_triangle));
  ASSERT_FALSE(triangle_with_right_symbolic_point->containsPoint(&point_inside_left_triangle));
}

// foo.h
class LegalEdgeChild : public DelaunayTriangulator {
 public:
  Vertex a = Vertex(-5, 0);
  Vertex b = Vertex(0, 5);
  Vertex c = Vertex(5, 0);
  Vertex d = Vertex(0, -5);

  bool isLegal(HalfEdgeRef he, HalfEdge::VertexRef s) { return isEdgeLegal(he, s); }
};

TEST(LegalEdgeTests, InitialTriangleTest) {
  auto legalEdge = LegalEdgeChild();

  // every edge of two_symbol_triangle should be legal
  auto initial_triangle = TriangleWithTwoSymbolicPoints(&legalEdge.d);
  auto edge             = initial_triangle.he;

  EXPECT_TRUE(legalEdge.isLegal(edge, &legalEdge.d));
  EXPECT_TRUE(legalEdge.isLegal(edge->getNext(), &legalEdge.d));
  EXPECT_TRUE(legalEdge.isLegal(edge->getNext()->getNext(), &legalEdge.d));

  // now add a neighbor triangle
  auto neighbor = TriangleWithOneSymbolicPoint(HalfEdge::Symbol::Right, &legalEdge.d, &legalEdge.c);

  auto initial_he  = initial_triangle.he->getNext();
  auto neighbor_he = neighbor.he->getPrev();

  initial_he->setTwin(neighbor_he);
  neighbor_he->setTwin(initial_he);

  auto point_in_initial_triangle = Vertex(-2, -7);
  auto point_in_neighbor         = Vertex(2.5, -5.0);

  EXPECT_FALSE(legalEdge.isLegal(initial_he, &point_in_initial_triangle));
  EXPECT_TRUE(legalEdge.isLegal(neighbor_he, &point_in_neighbor));
}

TEST(LegalEdgeTests, AllNonSymbolicPointsTest) {
  auto legalEdge = LegalEdgeChild();

  Triangle triangle1 = Triangle(&legalEdge.c, &legalEdge.a, &legalEdge.b);
  Triangle triangle2 = Triangle(&legalEdge.a, &legalEdge.c, &legalEdge.d);

  // test point is inside triangle2
  Vertex point_inside_circle  = Vertex(0, -2);
  Vertex point_outside_circle = Vertex(0, -6);

  triangle1.he->setTwin(triangle2.he);
  triangle2.he->setTwin(triangle1.he);

  EXPECT_FALSE(legalEdge.isLegal(triangle2.he, &point_inside_circle));
  EXPECT_TRUE(legalEdge.isLegal(triangle2.he, &point_outside_circle));
}
