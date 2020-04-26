cmake_minimum_required(VERSION 3.13.0)
project(draw_convex_hull_test)

set(CMAKE_CXX_STANDARD 14)

# 'lib' is the folder with Google Test sources
enable_testing()
find_package(GTest REQUIRED)

if (GTEST_FOUND)
    message(STATUS "googletest found!!")
else()
    message(STATUS "googletest not found:( use system googletest instead.")
endif()

if (NOT DEFINED ${GTest_DIR})
    set(GTest_DIR "/usr/src/googletest/googletest")
endif()

include_directories(../ ${GTest_DIR} ${GTest_DIR}/include)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(convex_hull_tests pointInPolygonTests.cpp ../src/Edge.cpp ../src/PointInPolygon.cpp )
target_link_libraries(convex_hull_tests GTest::GTest GTest::Main)

