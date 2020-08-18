cmake_minimum_required(VERSION 3.13.0)
project(draw_convex_hull_test)

set(CMAKE_CXX_STANDARD 14)

# 'lib' is the folder with Google Test sources
enable_testing()
find_package(GTest REQUIRED)

if (GTEST_FOUND)
    message(STATUS "googletest found!!")
else ()
    message(STATUS "googletest not found:( use system googletest instead.")
endif ()

if (NOT DEFINED ${GTest_DIR})
    set(GTest_DIR "/usr/src/googletest/googletest")
endif ()

include_directories(../ ${GTest_DIR} ${GTest_DIR}/include)

# one executable for each test suite
add_executable(convex_hull_tests
        convexHullTests.cpp
        ../src/FastConvexHull.cpp
        ../src/Edge.cpp
        ../src/PointInPolygon.cpp
        )

add_executable(point_in_polygon_tests
        pointInPolygonTests.cpp
        ../src/Edge.cpp
        ../src/PointInPolygon.cpp
        )

add_executable(dag_tests DirectedAcyclicGraphTests.cpp)

add_executable(math_tests GeometryAndMathTests.cpp)

target_link_libraries(convex_hull_tests GTest::GTest GTest::Main)
target_link_libraries(point_in_polygon_tests GTest::GTest GTest::Main)
target_link_libraries(dag_tests GTest::GTest GTest::Main)
target_link_libraries(math_tests GTest::GTest GTest::Main)

