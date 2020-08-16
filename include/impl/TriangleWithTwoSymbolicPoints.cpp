
TriangleWithTwoSymbolicPoints::TriangleWithTwoSymbolicPoints(VertexRef left, VertexRef p0,
                                                             VertexRef right)
    : Triangle(left, p0, right) {}

bool TriangleWithTwoSymbolicPoints::containsPoint(const VertexRef p) const { return *p < *b; }
