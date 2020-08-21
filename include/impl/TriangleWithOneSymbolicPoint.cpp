
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
  Vertex eta  = *b - *a;
  auto q      = static_cast<Vertex::VectorType>(*p) - *a;
  double side = eta.x * q.y - q.x * eta.y;

  switch (symbol) {
    case Vertex::Left:
      return side < 0 && (p < a) && (p > b);
    case Vertex::Right:
      return side > 0 && (a > p) && (b < p);
    default:
      return false;  // possibly identify if p is in the edge
  }
}

TriangleWithOneSymbolicPoint::~TriangleWithOneSymbolicPoint() = default;
