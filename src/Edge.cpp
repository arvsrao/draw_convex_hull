#include <include/Edge.h>

Edge::Edge(Vector2D _start, Vector2D _end) : start(_start), end(_end) {}

bool Edge::segmentCompare(Edge &e, Edge &f) {
  auto p = e.end - e.start;
  auto q = f.end - f.start;

  return p.arccos() < q.arccos();
}

bool Edge::operator==(const Edge &rhs) const {
  return (rhs.start == start) && (rhs.end == end);
}

Edge::~Edge() {}
