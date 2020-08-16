
#include <HalfEdge.h>

HalfEdge::HalfEdge(VertexRef _origin)
    : origin(_origin), next(nullptr), prev(nullptr), twin(nullptr) {}

HalfEdge::HalfEdge(VertexRef _origin, HalfEdgeRef _prev, HalfEdgeRef _next, HalfEdgeRef _twin)
    : origin(_origin), prev(_prev), next(_next), twin(_twin) {}

Vertex* HalfEdge::getOrigin() { return origin; }

HalfEdge* HalfEdge::getTwin() { return twin; }

HalfEdge* HalfEdge::getNext() { return next; }

void HalfEdge::setPrev(HalfEdge* he) { prev = he; }

void HalfEdge::setNext(HalfEdge* he) { next = he; }

void HalfEdge::setTwin(HalfEdge* he) { twin = he; }

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
