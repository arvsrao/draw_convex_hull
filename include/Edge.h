#ifndef DRAW_CONVEX_HULL_EDGE_H
#define DRAW_CONVEX_HULL_EDGE_H

#include <include/Vector2D.h>

using Point  = Vector2D<int>;

struct Edge {
  Point start, end;

  Edge(Point _start, Point _end);

  static bool segmentCompare(Edge &e, Edge &f);

  bool operator==(const Edge &rhs) const;

  virtual ~Edge();
};

#endif //DRAW_CONVEX_HULL_EDGE_H
