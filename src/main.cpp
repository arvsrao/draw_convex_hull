#include <set>
#include <string>
#include <random>
#include <iostream>

#include <QGraphicsView>
#include <QtWidgets/QApplication>

#include <include/SlowConvexHull.h>
#include <include/FastConvexHull.h>

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

ConvexHull *convexHullFactory(bool fast_bool, Points &points) {
  if (fast_bool)
    return new FastConvexHull(points);
  return new SlowConvexHull(points);
}

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  int num_points = 0;
  bool fast_bool = true;

  if (argc < 5) {
    std::cout << "Required parameters: \n"
              << "--num  [Int] number of points to generate\n"
              << "--fast [Boolean] 'true' for n*log(n) implementation of convex hull generation\n"
              << "Ex.\n"
              << " draw_convex_hull_demo --num 10000 --fast true\n";

    return 0;
  }

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i],"--num")==0)
      num_points = std::atoi(argv[i + 1]);
    else if (strcmp(argv[i], "--fast") == 0) {
      if (strcmp(argv[i + 1],"true") == 0)
        fast_bool = true;
      else
        fast_bool = false;
    }
  }

  auto _points = generatePoints(num_points, 200);

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