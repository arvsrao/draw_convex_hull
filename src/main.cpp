#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <include/FastConvexHull.h>
#include <QVector>

Points generatePoints(const int numPoints, const int maxRange) {
  std::random_device rd;
  std::default_random_engine generator(rd());
  std::uniform_int_distribution<int> distribution(-maxRange, maxRange);

  Points retVal;
  for (int i = 0; i < numPoints; i++) {
    auto pt = Vector2D(distribution(generator), distribution(generator));
    if ((pt.x * pt.x / 30.0) + (pt.y * pt.y / 100.0) < maxRange) retVal.push_back(pt);
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

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  const int NUM_POINTS = 10000;
  auto _points = generatePoints(NUM_POINTS, 200);

  // Instantiate convex hull calculator
  FastConvexHull *convexHullCalculator = new FastConvexHull(_points);

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


//  if (hull.size() != shull.size()) std::cout<< "wrong!!!! " << hull.size() - shull.size();

//    printPath(hull);
//    std::cout<< "--------------\n";
//    printPath(shull);
