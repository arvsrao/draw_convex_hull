#include <include/FastConvexHull.h>
#include <include/PointInPolygon.h>

#include <QGraphicsView>
#include <QtWidgets/QApplication>
#include <random>
#include <set>

Points polyLine = {Point(-44, -134), Point(-67, -112), Point(-105, -39), Point(-64, 42),
                   Point(5, 96),     Point(93, 47),    Point(121, -15),  Point(99, -83),
                   Point(85, -106),  Point(61, -60),   Point(63, -14),   Point(34, 40),
                   Point(-14, 22),   Point(-46, -33),  Point(-22, -65),  Point(-33, -107)};

Points generatePoints(const int numPoints, const double maxRange) {
  std::random_device rd;
  std::default_random_engine generator(rd());

  std::uniform_real_distribution<double> distribution(-maxRange, maxRange);
  PointInPolygon pipCalc = PointInPolygon(polyLine);

  Points retVal;
  int i = 0;
  while (i < numPoints) {
    auto pt = Vector2D<int>(distribution(generator), distribution(generator));

    if (pipCalc.pointInPolygon(pt)) {
      retVal.push_back(pt);
      i++;
    }
  }

  // remove duplicates
  auto last = std::unique(retVal.begin(), retVal.end());
  retVal.erase(last, retVal.end());

  return retVal;
}

void display(Points &_points, Points &hull, QGraphicsScene *scene) {
  QVector<QPointF> points;

  // Fill in points with n number of points
  for (unsigned long i = 0; i < _points.size(); i++)
    points.append(QPointF(_points[i].x, _points[i].y));

  for (long i = 0; i < points.size(); i++) scene->addEllipse(points[i].x(), points[i].y(), 1, 1);

  // print the given shape boundary
  for (unsigned long i = 0; i < polyLine.size(); i += 1)
    scene->addLine(polyLine[i].x, polyLine[i].y, polyLine[(i + 1) % polyLine.size()].x,
                   polyLine[(i + 1) % polyLine.size()].y, QPen(Qt::green, 2));

  // print the computed convex hull
  for (unsigned long i = 0; i < hull.size(); i += 1) {
    scene->addLine(hull[i].x, hull[i].y, hull[(i + 1) % hull.size()].x,
                   hull[(i + 1) % hull.size()].y, QPen(Qt::blue, 2));
  }
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  const int num_points = 5000;

  auto _points = generatePoints(num_points, 200.0);

  // Instantiate convex hull calculator
  FastConvexHull *convexHullCalculator = new FastConvexHull(_points);

  // Create a view, put a scene in it and add tiny circles
  // in the scene
  QGraphicsView *view   = new QGraphicsView();
  QGraphicsScene *scene = new QGraphicsScene();
  view->setScene(scene);

  // Show the view
  display(_points, convexHullCalculator->getConvexHull(), scene);
  view->show();

  delete convexHullCalculator;

  return a.exec();
}
