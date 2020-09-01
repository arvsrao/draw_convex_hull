#include <TriangleWithTwoSymbolicPoints.h>

TriangleWithTwoSymbolicPoints::TriangleWithTwoSymbolicPoints(VertexRef p)
    : Triangle(nullptr, p, nullptr) {
  // fixed and arbitrary choice. all half edges are symbolic anyway.
  orientation = unset;
}

bool TriangleWithTwoSymbolicPoints::containsPoint(VertexRef p) const { return *p < *b; }

Triangle::Orientation TriangleWithTwoSymbolicPoints::getOrientation() { return orientation; }

void TriangleWithTwoSymbolicPoints::setOrientation() { orientation = unset; }
