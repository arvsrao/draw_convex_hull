#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <include/Vector2D.h>

constexpr double PI = 3.14159265358979323846;

Vector2D::Vector2D() : x(0), y(0) {}
Vector2D::Vector2D(int _x, int _y) : x(_x), y(_y) {}

inline double Vector2D::length() const {
  return std::sqrt((double) (x * x + y * y));
}

inline double Vector2D::arccos() const {
  // dot with (-1,0)
  if (y < 0)
    return std::acos(double(x) / this->length()) + PI;
  return std::acos(double(-x) / this->length());
}

bool Vector2D::compareVector2D(Vector2D &a, Vector2D &b) {
  return (a.x == b.x) ? a.y < b.y : a.x < b.x;
}

bool Vector2D::operator==(const Vector2D &rhs) const {
  return (rhs.x == x) && (rhs.y == y);
}

bool Vector2D::operator!=(const Vector2D &rhs) const {
  return !(*this == rhs);
}

Vector2D Vector2D::operator-(const Vector2D &rhs) const {
  return Vector2D(x - rhs.x, y - rhs.y);
}

void Vector2D::print() {
  std::cout << "(" << x << ", " << y << ")" << "\n";
}

Vector2D::~Vector2D() {}

