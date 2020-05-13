# draw_convex_hull
![clang-format and tests][1]

A QT program that draws the convex hull of randomly generated points.
* Requires at least CMake 3.13 to compile and build.
* The project depends on [Qt5][2] and [GoogleTest][3] installations. 


#### Install Dependencies

###### GoogleTest

On MacOS I find it most convenient to install GoogleTest system-wide as opposed to directly including
the ``googletest/include``` directory in the project. On MacOS clone [GoogleTest][3] and build with CMake like so

```
   git clone https://github.com/google/googletest.git
   mkdir build
   cd build
   cmake ../ && make && make install
```

On Pop_OS! (which is based on Ubuntu) I don't formally build the project and install system-wide, instead 
I clone the project and simply move to a place like ```/usr/src/``` where its visible in the system search 
PATH.

###### Qt5

On Pop_OS! install Qt5 with ```sudo apt-get -y install qtbase5-dev```. On MacOS install 
with ```brew install qt```, then sym

#### Capabilities

The built executable creates a configurable convex hull demo. The user can
 * choose the number of randomly generated points from which a convex hull is generated
 * pick a fast, ( n * log(n) ) convex hull algorithm, or the quadratic implementation.
 
```
Required parameters: 
--num  [Int] number of points to generate
--fast [Boolean] 'true' for the n*log(n) implemenation of convex hull algorithm
Ex.
 draw_convex_hull_demo --num 10000 --fast true
```

![Image description](resources/demo-convex-hull.jpg)

* Given a simple closed 2D polygon generate a point-in-polygon predicate. In the image below one sees several thousand randomly
generated points that were masked (or filtered) by ```PointInPolygon``` of the polygon marked in green. For comparison, 
the convex hull of the polygon is shown in blue.

![PointInPolygon](resources/point-in-polygon-concave.png)

[1]: https://github.com/arvsrao/draw_convex_hull/workflows/CI/badge.svg
[2]: https://github.com/qt/qt5
[3]: https://github.com/google/googletest
