//
// Created by arvind on 20.03.20.
//

#ifndef DRAW_CONVEX_HULL_EDGE_H
#define DRAW_CONVEX_HULL_EDGE_H

struct Edge {
  Vector2D start, end;

  Edge(Vector2D _start, Vector2D _end);

  static bool segmentCompare(Edge &e, Edge &f);

  bool operator==(const Edge &rhs) const;

  virtual ~Edge();
};

#endif //DRAW_CONVEX_HULL_EDGE_H
