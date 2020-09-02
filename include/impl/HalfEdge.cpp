
#include <HalfEdge.h>
#include <SquareMatrix.h>

HalfEdge::HalfEdge(Vertex::Symbol symbol)
    : origin(nullptr), symbol(symbol), next(nullptr), prev(nullptr), twin(nullptr) {}

HalfEdge::HalfEdge(VertexRef origin)
    : origin(origin), symbol(Vertex::None), next(nullptr), prev(nullptr), twin(nullptr) {}

HalfEdge::HalfEdge(VertexRef origin, HalfEdgeRef prev, HalfEdgeRef next, HalfEdgeRef twin)
    : origin(origin), symbol(Vertex::None), prev(prev), next(next), twin(twin) {}

HalfEdge::VertexRef HalfEdge::getOrigin() const { return origin; }

HalfEdge::HalfEdgeRef HalfEdge::getTwin() const { return twin; }

HalfEdge* HalfEdge::getNext() const { return next; }

HalfEdge* HalfEdge::getPrev() const { return prev; }

Vertex::Symbol HalfEdge::getSymbol() const { return symbol; }

void HalfEdge::setPrev(HalfEdge* he) { prev = he; }

void HalfEdge::setNext(HalfEdge* he) { next = he; }

void HalfEdge::setTwin(HalfEdge* he) { twin = he; }

HalfEdge::TriangleRef HalfEdge::getTriangleRef() const { return triangleRef; }

void HalfEdge::setTriangleRef(HalfEdge::TriangleRef triangleRef) { triangleRef = triangleRef; }

bool HalfEdge::hasSymbol() const { return symbol != Vertex::None; }

bool HalfEdge::isVertexInHalfEdge(HalfEdge::VertexRef b) {
  HalfEdge::VertexRef a = next->getNext()->origin;

  std::array<Vertex::RingType, 9> mat = {
      origin->x, origin->y, 1,  //
      a->x,      a->y,      1,  //
      b->x,      b->y,      1,  //
  };
  return std::abs(SquareMatrix<3, Vertex::RingType>(mat).det()) < 1e-6;
}

HalfEdge::~HalfEdge() {
  // Do NOT delete the origin because it is owned by vertex list.
  if (prev) prev->setNext(nullptr);
  std::cout << "delete half edge (" << origin->x << ", " << origin->y << ") -> ";
  if (next)
    std::cout << "(" << next->getOrigin()->x << ", " << next->getOrigin()->y << ")\n";
  else
    std::cout << "(x_0,y_0)\n";
  delete next;
}

HalfEdge::TriangleRef HalfEdge::getTriangleRef() const { return triangleRef; }
void HalfEdge::setTriangleRef(HalfEdge::TriangleRef triangleRef) { triangleRef = triangleRef; }

bool HalfEdge::hasSymbol() const { return symbol != Vertex::None; }
