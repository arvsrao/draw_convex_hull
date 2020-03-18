#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <set>
#include <string>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPointF>
#include <QVector>

struct Vector2D {
    int x, y;

    Vector2D() : x(0), y(0) {}
    Vector2D(int _x, int _y): x(_x), y(_y) {}

    double length() {
        return sqrt((double)(x*x +y*y));
    }

    bool operator==(const Vector2D& rhs) const {
        return (rhs.x == x) && (rhs.y == y);
    }

    bool operator!=(const Vector2D& rhs) const {
        return !(*this == rhs);
    }

    Vector2D operator-(const Vector2D& rhs) const {
        return Vector2D(x - rhs.x, y - rhs.y);
    }

    void print() {
        std::cout << "(" << x << ", " << y << ")" << "\n";
    }
};

struct Edge {
    Vector2D start, end;

    Edge(Vector2D _start, Vector2D _end): start(_start), end(_end) {}

    bool operator==(const Edge& rhs) const {
        return (rhs.start == start) && (rhs.end == end);
    }
};

using Points = std::vector<Vector2D>;

/*
*
* @param p: is a fixed orient segment under consideration
* when return is false, @param q is to the left of @param p
*
*  det [p.x p.y]
*      [q.x q.y]
*
*  OR
*
*  the right _|_ vector of p is (p.y, -p.x)
*
*  (q.x,q.y) * (p.y, -p.x) = q.x * p.y - q.y * p.x > 0
*
*/
bool isNegativeFrame(const Vector2D& p , const Vector2D& q) {
    return (p.x * q.y - q.x * p.y) < 0;
}

// do three points make a right turn.
// same as asking if 'end' lies to the right of
// segment start-middle
bool rightTurn(const Vector2D& start , const Vector2D& middle, const Vector2D& end) {
    return isNegativeFrame(middle - start, end - start);
}

Points generatePoints(const int numPoints, const int maxRange) {
   std::random_device rd;
   std::default_random_engine generator(rd());
   std::uniform_int_distribution<int> distribution(-maxRange, maxRange);

   Points retVal;
   for(int i = 0; i < numPoints; i++) {
       auto pt = Vector2D(distribution(generator), distribution(generator));
       if ( (pt.x * pt.x / 30.0) + (pt.y * pt.y / 100.0) < maxRange) retVal.push_back(pt);
   }

   // remove duplicates
   auto last = std::unique(retVal.begin(), retVal.end());
   retVal.erase(last, retVal.end());

   return retVal;
}

bool allOnRight(const Vector2D& p, const Vector2D& q, const Points& points) {
    for (size_t i = 0; i < points.size(); i++) {
        auto v = points[i];
        if ( q == v  || p == v) continue;
        if (!isNegativeFrame(q-p, v-p)) return false;
    }
    return true;
}

double arccos(Vector2D& p) {
    // dot with (-1,0)
    if (p.y < 0 )
        return acos( double(p.x) / p.length() ) + 3.14159265;
    return acos( double(-p.x) / p.length() );
}

bool segmentCompare(Edge& e, Edge& f) {
    auto p = e.end - e.start;
    auto q = f.end - f.start;

    return arccos(p) < arccos(q);
}

Points sortClockwise(std::vector<Edge>& segments) {
    std::sort(segments.begin(), segments.end(), segmentCompare);
    Points pts;
    for(unsigned long i = 0; i < segments.size(); i++) pts.push_back(segments[i].start);

    // remove duplicates.
//    auto last = unique(pts.begin(), pts.end());
//    pts.erase(last, pts.end());
    return pts;
}

Points slowConvexHull(const Points& points) {
    std::vector<Edge> hull;

    for (auto p = points.begin(); p != points.end(); p++) {
        for (auto q = points.begin(); q != points.end(); q++) {
            if (*p == *q) continue;
            if (allOnRight(*p,*q,points)) {
                hull.push_back(Edge(*p,*q));
            }
        }
    }
    return sortClockwise(hull);
}

bool compareVector2D(Vector2D& a, Vector2D& b) {
    return (a.x == b.x) ?  a.y < b.y : a.x < b.x;
}

Points _convexHull(const Points& points) {
    Points _hull;

    for(auto it = points.begin(); it != points.end(); it++) {
        _hull.push_back(*it);
        while (_hull.size() > 2) {
            auto c = _hull.back(); _hull.pop_back();
            auto b = _hull.back(); _hull.pop_back();
            auto a = _hull.back();
            if (!rightTurn(a, b, c)) _hull.push_back(c);
            else {
                _hull.push_back(b); _hull.push_back(c);
                break;
            }
        }
    }
    return _hull;
}

Points convexHull(Points& points) {
    std::sort(points.begin(), points.end(), compareVector2D);
    auto hull = _convexHull(points);

    std::sort(points.begin(), points.end(), [&](Vector2D& a, Vector2D& b){return (a.x == b.x) ?  a.y > b.y : a.x > b.x;});
    auto bottom = _convexHull(points);

    // concatenate top and bottom hulls.
    hull.pop_back();
    hull.insert(hull.end(), bottom.begin(), bottom.end()-1);

    return hull;
}

void display(Points _points, Points hull, QColor color) {

    QVector <QPointF> points;

    // Fill in points with n number of points
    for(unsigned long i = 0; i< _points.size(); i++) points.append(QPointF(_points[i].x, _points[i].y));

    // Create a view, put a scene in it and add tiny circles
    // in the scene
    QGraphicsView * view = new QGraphicsView();
    QGraphicsScene * scene = new QGraphicsScene();
    view->setScene(scene);

    for(long i = 0; i< points.size(); i++)
        scene->addEllipse(points[i].x(), points[i].y(), 1, 1);

    for(unsigned long i = 0; i< hull.size(); i+=1)
        scene->addLine(hull[i].x,hull[i].y,hull[(i+1) % hull.size()].x,hull[(i+1) % hull.size()].y, QPen(color));

    // Show the view
    view->show();
}

void printPath(Points points) {
    for (unsigned long i = 0; i < points.size(); i++) points[i].print();
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    const int NUM_POINTS = 500;
    auto _points = generatePoints(NUM_POINTS,100);

    //compute convex hull
     Points hull = convexHull(_points);
//     Points shull = slowConvexHull(_points);

   //  if (hull.size() != shull.size()) std::cout<< "wrong!!!! " << hull.size() - shull.size();

//    printPath(hull);
//    std::cout<< "--------------\n";
//    printPath(shull);

    display(_points, hull, Qt::blue);
//    display(_points, shull, Qt::red);
    // or add the view to the layout inside another widget
    return a.exec();
}
