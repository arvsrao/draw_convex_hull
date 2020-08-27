#include <SquareMatrix.h>
#include <TriangleWithOneSymbolicPoint.h>

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint()
    : Triangle(nullptr, nullptr, nullptr), symbol(Vertex::Symbol::None) {}

TriangleWithOneSymbolicPoint::TriangleWithOneSymbolicPoint(VertexRef symbolicVertex, VertexRef a,
                                                           VertexRef b)
    : Triangle(a, b, symbolicVertex) {
  symbol = symbolicVertex ? symbolicVertex->getSymbol() : Vertex::Symbol::None;
}

Triangle::HalfEdgeRef TriangleWithOneSymbolicPoint::halfEdgeContainsPoint(Triangle::VertexRef p) {
  return he->isVertexInHalfEdge(p) ? he : nullptr;
}

// true if the given vertex is to right of edge ab.
bool TriangleWithOneSymbolicPoint::containsPoint(const VertexRef p) const {
  auto a = he->getOrigin();
  auto b = he->getNext()->getOrigin();

  // swap if a is not higher/greater than b
  if (*a < *b) std::swap(a, b);

  std::array<Vertex::RingType, NUM_VERTICES_PER_FACE * NUM_VERTICES_PER_FACE> mat{
      p->x, p->y, 1,  //
      a->x, a->y, 1,  //
      b->x, b->x, 1,  //
  };

  auto side = SquareMatrix<NUM_VERTICES_PER_FACE, Vertex::RingType>(mat).det();

  switch (symbol) {
    case Vertex::Left:
      return side < 0 && (*p < *a) && (*p > *b);
    case Vertex::Right:
      return side > 0 && (*p < *a) && (*p > *b);
    default:
      return false;  // possibly identify if p is in the edge
  }
}

TriangleWithOneSymbolicPoint::~TriangleWithOneSymbolicPoint() = default;
