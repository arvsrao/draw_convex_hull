#include <include/Vector2D.h>

#include <algorithm>
#include <random>

using Point  = Vector2D<int>;
using Points = std::vector<Point>;

namespace simulation {

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

}  // namespace simulation
