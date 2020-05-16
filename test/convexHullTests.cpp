#include <gtest/gtest.h>
#include <include/PointInPolygon.h>

#include <include/util/simulation.cpp>

#include "include/FastConvexHull.h"

// Generate 100 points check that points are all within computed hull
Points punkt_menge = simulation::generatePoints(100, 300.0);

auto convexHullCalculator = FastConvexHull(punkt_menge);
auto hull                 = convexHullCalculator.getConvexHull();

bool isNegativeFrame(const Point &p, const Point &q) {
  return (p.x * q.y - q.x * p.y) < 0;
}

TEST(ConvexHull, GenerationTest) {
  PointInPolygon pipCalc = PointInPolygon(hull);

  // all points in the cloud should be inside or on the hull
  for (auto &punkt : punkt_menge) {
    ASSERT_TRUE(pipCalc.pointInPolygon(punkt));
  }
}

TEST(ConvexHull, convexityTest) {
  for (int idx = 0; idx < hull.size(); idx++) {
    auto vec1 = hull[idx + 1 % hull.size()] - hull[idx];
    auto vec2 = hull[idx + 2 % hull.size()] - hull[idx + 1 % hull.size()];

    ASSERT_TRUE(isNegativeFrame(vec1, vec2));
  }
}
