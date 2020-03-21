cmake_minimum_required(VERSION 3.13.0)
project(draw_convex_hull)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "~/Qt/5.14.1/gcc_64/lib/cmake/Qt5Widgets/")

find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

include_directories(include)

add_executable(fast_convex_hull_demo src/main.cpp src/Vector2D.cpp src/FastConvexHull.cpp)

target_link_libraries(fast_convex_hull_demo Qt5::Widgets)