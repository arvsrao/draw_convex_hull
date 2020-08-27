#ifndef DRAW_CONVEX_HULL_VERTEX_H
#define DRAW_CONVEX_HULL_VERTEX_H
#include <Vector2D.h>

// A Vertex is a Vector2D<double> that can optionally be a symbolic point
class Vertex : public Vector2D<double> {
 public:
  // None : the vertex is not a symbolic point
  // Left : the vertex is symbolic and to the left && above all other vertices
  // Right : the vertex is symbolic and to the right && below all other vertices
  enum Symbol { Left = +1, None = 0, Right = -1 };

  Vertex();
  Vertex(Symbol _symbol);
  Vertex(double _x, double _y);
  Vertex(double _x, double _y, Symbol _symbol);

  void setLeft();
  void setRight();

  Symbol getSymbol() const;

  bool isSymbol() const;
  bool isLeft() const;
  bool isRight() const;

  Vertex operator-(const Vertex &rhs) const;

  bool operator==(const Vertex &rhs) const;

  bool operator!=(const Vertex &rhs) const;

  bool operator<(const Vertex &rhs) const;

  bool operator>(const Vertex &rhs) const;

  bool operator>=(const Vertex &rhs) const;

 private:
  Symbol symbol;
};

#include <impl/Vertex.cpp>

#endif  // DRAW_CONVEX_HULL_VERTEX_H
