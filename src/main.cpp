#include <set>
#include <string>
#include <random>
#include <iostream>

#include <QGraphicsView>
#include <QtWidgets/QApplication>

#include <include/CLI11.hpp>
#include <include/SlowConvexHull.h>
#include <include/FastConvexHull.h>

Points generatePoints(const int numPoints, const int maxRange) {
  std::random_device rd;
  std::default_random_engine generator(rd());

  std::normal_distribution<double> distribution(0.0, (double) maxRange / 2.0);

  Points retVal;
  int i = 0;
  while (i < numPoints) {
    auto pt = Vector2D<int>(distribution(generator), distribution(generator));

    if (pt.length() > (double) maxRange / 4.0 && pt.length() < 3.0 * (double) maxRange / 4.0) {
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
  for (unsigned long i = 0; i < _points.size(); i++) points.append(QPointF(_points[i].x, _points[i].y));

  for (long i = 0; i < points.size(); i++)
    scene->addEllipse(points[i].x(), points[i].y(), 1, 1);

  for (unsigned long i = 0; i < hull.size(); i += 1)
    scene->addLine(hull[i].x,
                   hull[i].y,
                   hull[(i + 1) % hull.size()].x,
                   hull[(i + 1) % hull.size()].y,
                   QPen(Qt::blue, 2));
}

void printPath(Points points) {
  for (unsigned long i = 0; i < points.size(); i++) points[i].print();
}

ConvexHull *convexHullFactory(bool fast_bool, Points &points) {
  if (fast_bool)
    return new FastConvexHull(points);
  return new SlowConvexHull(points);
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  CLI::App app{"Convell Hull Demo"};

  int num_points = 0;
  app.add_option("--num", num_points, "number of points to generate")->required();

  bool fast_bool = true;
  app.add_option("--fast",
                 fast_bool,
                 "'true' for n*log(n) implementation of convex hull generation");

  try {
    app.parse(argc, argv);
  } catch (const CLI::ParseError &e) {
    return app.exit(e);
  }

  auto _points = generatePoints(num_points, 250);

  // Instantiate convex hull calculator
  ConvexHull *convexHullCalculator = convexHullFactory(fast_bool, _points);

  // Create a view, put a scene in it and add tiny circles
  // in the scene
  QGraphicsView *view = new QGraphicsView();
  QGraphicsScene *scene = new QGraphicsScene();
  view->setScene(scene);

  // Show the view
  display(_points, convexHullCalculator->getConvexHull(), scene);
  view->show();

  delete convexHullCalculator;

  return a.exec();
}