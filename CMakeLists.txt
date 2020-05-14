cmake_minimum_required(VERSION 3.13.0)
project(draw_convex_hull)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Ubuntu ~/Qt/5.14.1/gcc_64/lib/cmake/Qt5Widgets/
# MacOS /usr/local/Cellar/qt/5.14.2/include/QtWidgets/
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)

add_executable(draw_convex_hull_demo src/main.cpp src/Edge.cpp src/FastConvexHull.cpp src/SlowConvexHull.cpp)
add_executable(concaveHull src/concave2DPointCloud.cpp src/Edge.cpp src/FastConvexHull.cpp src/PointInPolygon.cpp)

target_link_libraries(draw_convex_hull_demo Qt5::Widgets)
target_link_libraries(concaveHull Qt5::Widgets)

add_subdirectory(test)
