
#include <HalfEdge.h>

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

bool HalfEdge::hasSymbol() const { return symbol != Vertex::None; }

bool HalfEdge::isVertexInHalfEdge(HalfEdge::VertexRef vertexRef) {
  HalfEdge::VertexRef b = next->getNext()->origin;

  //     [ origin->x origin->y       1 ]
  // det [  b->x      b->y           1 ]  = 0 if collinear
  //     [ vertexRef->x vertexRef->y 1 ]
  auto det = origin->x * (b->y - vertexRef->y) - origin->y * (b->x - vertexRef->x) +
             (b->x * vertexRef->y - b->y * vertexRef->x);
  return det == 0;
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
