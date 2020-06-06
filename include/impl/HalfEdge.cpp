
template <typename T>
HalfEdge<T>::HalfEdge(VertexRef _origin)
    : origin(_origin), next(nullptr), prev(nullptr), twin(nullptr) {}

template <typename T>
HalfEdge<T>::HalfEdge(VertexRef _origin, HalfEdgeRef _prev, HalfEdgeRef _next, HalfEdgeRef _twin)
    : origin(_origin), prev(_prev), next(_next), twin(_twin) {}

template <typename T>
Vector2D<T>* HalfEdge<T>::getOrigin() {
  return origin;
}

template <typename T>
HalfEdge<T>* HalfEdge<T>::getTwin() {
  return twin;
}

template <typename T>
HalfEdge<T>* HalfEdge<T>::getNext() {
  return next;
}

template <typename T>
void HalfEdge<T>::setPrev(HalfEdge<T>* he) {
  prev = he;
}

template <typename T>
void HalfEdge<T>::setNext(HalfEdge<T>* he) {
  next = he;
}

template <typename T>
void HalfEdge<T>::setTwin(HalfEdge<T>* he) {
  twin = he;
}

template <typename T>
HalfEdge<T>::~HalfEdge() {
  // Do NOT delete the origin because it is owned by vertex list.
  if (prev) prev->setNext(nullptr);
  std::cout << "delete half edge (" << origin->x << ", " << origin->y << ") -> ";
  if (next)
    std::cout << "(" << next->getOrigin()->x << ", " << next->getOrigin()->y << ")\n";
  else
    std::cout << "(x_0,y_0)\n";
  delete next;
}