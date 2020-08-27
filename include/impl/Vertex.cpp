
Vertex::Vertex() : Vector2D<double>(0, 0), symbol(Vertex::Symbol::None) {}

Vertex::Vertex(Vertex::Symbol _symbol) : Vector2D<double>(0, 0), symbol(_symbol) {}

Vertex::Vertex(double _x, double _y) : Vector2D<double>(_x, _y), symbol(Vertex::Symbol::None) {}

Vertex::Vertex(double _x, double _y, Vertex::Symbol _symbol)
    : Vector2D<double>(_x, _y), symbol(_symbol) {}

void Vertex::setLeft() { symbol = Vertex::Symbol::Left; }
void Vertex::setRight() { symbol = Vertex::Symbol::Right; }

Vertex::Symbol Vertex::getSymbol() const { return symbol; }

bool Vertex::isSymbol() const { return symbol != Vertex::Symbol::None; }
bool Vertex::isLeft() const { return symbol == Vertex::Symbol::Left; }
bool Vertex::isRight() const { return symbol == Vertex::Symbol::Right; }

Vertex Vertex::operator-(const Vertex &rhs) const {
  return Vertex(x - rhs.x, y - rhs.y, Symbol::None);
}

bool Vertex::operator==(const Vertex &rhs) const {
  if (isSymbol() || rhs.isSymbol())
    return symbol == rhs.symbol;
  else
    return VectorType::operator==(rhs);
}

bool Vertex::operator!=(const Vertex &rhs) const { return !(*this == rhs); }

bool Vertex::operator<(const Vertex &rhs) const { return !(*this >= rhs); }

bool Vertex::operator>=(const Vertex &rhs) const { return (*this > rhs) || (*this == rhs); }

bool Vertex::operator>(const Vertex &rhs) const {
  if (isSymbol() || rhs.isSymbol())
    return symbol > rhs.symbol;
  else
    return VectorType::operator>(rhs);
}
