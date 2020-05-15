#include <gtest/gtest.h>
#include <include/PointInPolygon.h>

#include <random>

#include "include/FastConvexHull.h"

Points generatePoints(const int numPoints, const double maxRange) {
  std::random_device rd;
  std::default_random_engine generator(rd());

  std::normal_distribution<double> distribution(0.0, maxRange / 2.0);

  Points retVal;
  int i = 0;
  while (i < numPoints) {
    auto pt = Vector2D<int>(distribution(generator), distribution(generator));

    if (pt.length() > maxRange / 4.0 && pt.length() < 3.0 * maxRange / 4.0) {
      retVal.push_back(pt);
      i++;
    }
  }

  // remove duplicates
  auto last = std::unique(retVal.begin(), retVal.end());
  retVal.erase(last, retVal.end());

  return retVal;
}

TEST(ConvexHull, GenerationTest) {
  // Generate 100 points check that points are all within computed hull
  Points punkt_menge = generatePoints(100, 300.0);

  auto convexHullCalculator = FastConvexHull(punkt_menge);
  auto hull                 = convexHullCalculator.getConvexHull();
  PointInPolygon pipCalc    = PointInPolygon(hull);

  for (auto &punkt : punkt_menge) {
    ASSERT_TRUE(pipCalc.pointInPolygon(punkt));
  }
}
