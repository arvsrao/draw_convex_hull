//
// Created by arvind on 20.03.20.
//
#include <include/Vector2D.h>
#include <include/Edge.h>

Edge::Edge(Vector2D _start, Vector2D _end) : start(_start), end(_end) {}

static bool Edge::segmentCompare(Edge &e, Edge &f) {
  auto p = e.end - e.start;
  auto q = f.end - f.start;

  return p.arccos() < q.arccos();
}

bool Edge::operator==(const Edge &rhs) const {
  return (rhs.start == start) && (rhs.end == end);
}

Edge::~Edge() {
  delete start;
  delete end;
}
