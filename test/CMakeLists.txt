cmake_minimum_required(VERSION 3.13.0)
project(draw_convex_hull_test)

set(CMAKE_CXX_STANDARD 14)

# 'lib' is the folder with Google Test sources
add_subdirectory(gtest)
include_directories(gtest/include gtest)
include_directories(../ project)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(convex_hull_tests pointInPolygonTests.cpp ../src/Edge.cpp ../src/PointInPolygon.cpp )
target_link_libraries(convex_hull_tests gtest gtest_main)

