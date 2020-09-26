
#include <HalfEdge.h>
#include <SquareMatrix.h>

HalfEdge::HalfEdge(Symbol symbol)
    : origin(nullptr), symbol(symbol), next(nullptr), prev(nullptr), twin(nullptr) {}

HalfEdge::HalfEdge(VertexRef origin)
    : origin(origin), symbol(None), next(nullptr), prev(nullptr), twin(nullptr) {}

HalfEdge::HalfEdge(VertexRef origin, HalfEdgeRef prev, HalfEdgeRef next, HalfEdgeRef twin)
    : origin(origin), symbol(None), prev(prev), next(next), twin(twin) {}

HalfEdge::VertexRef HalfEdge::getOrigin() const { return origin; }

HalfEdge::HalfEdgeRef HalfEdge::getTwin() const { return twin; }

HalfEdge* HalfEdge::getNext() const { return next; }

HalfEdge* HalfEdge::getPrev() const { return prev; }

HalfEdge::Symbol HalfEdge::getSymbol() const { return symbol; }

void HalfEdge::setPrev(HalfEdge* he) { prev = he; }

void HalfEdge::setNext(HalfEdge* he) { next = he; }

void HalfEdge::setTwin(HalfEdge* he) { twin = he; }

void HalfEdge::setSymbol(HalfEdge::Symbol s) { symbol = s; }

HalfEdge::TriangleRef HalfEdge::getTriangleRef() const { return triangleRef; }

void HalfEdge::setTriangleRef(HalfEdge::TriangleRef ref) { triangleRef = ref; }

bool HalfEdge::hasSymbol() const { return symbol != None; }

bool HalfEdge::isVertexInHalfEdge(HalfEdge::VertexRef b) {
  // assume that b != origin or any other Vertex
  if (!hasSymbol() && !next->hasSymbol()) {
    VertexRef a = next->getOrigin();

    std::array<RingType, 9> mat = {
        origin->x, origin->y, 1,  //
        a->x,      a->y,      1,  //
        b->x,      b->y,      1,  //
    };
    return std::abs(SquareMatrix<3, RingType>(mat).det()) < 1e-6;
  }
  return false;
}

HalfEdge::~HalfEdge() {
  // Do NOT delete the origin because it is owned by vertex list.
  if (origin) std::cout << "delete half edge (" << origin->x << ", " << origin->y << ") -> ";
  if (next) {
    if (next->getSymbol() == Left)
      std::cout << "LEFT"
                << "\n";
    else if (next->getSymbol() == Right)
      std::cout << "Right"
                << "\n";
    else
      std::cout << "(" << next->getOrigin()->x << ", " << next->getOrigin()->y << ")\n";
  }
  if (prev) prev->setNext(nullptr);
  delete next;
}
