
TriangleWithTwoSymbolicPoints::TriangleWithTwoSymbolicPoints(VertexRef p)
    : Triangle(nullptr, p, nullptr) {}

bool TriangleWithTwoSymbolicPoints::containsPoint(VertexRef p) const { return *p < *b; }
