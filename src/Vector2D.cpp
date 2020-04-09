#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>
#include <include/Vector2D.h>

const double PI = 3.14159265358979323846;

template<Typename T>
Vector2D<T>::Vector2D() : x(0), y(0) {}

template<Typename T>
Vector2D<T>::Vector2D(T _x, T _y) : x(_x), y(_y) {}

template<Typename T>
double Vector2D<T>::length() const {
  return std::sqrt((double) (x * x + y * y));
}

template<Typename T>
double Vector2D<T>::arccos() const {
  // dot with (-1,0)
  if (y < (T)0)
    return std::acos(double(x) / this->length()) + PI;
  return std::acos(double(-x) / this->length());
}

template<Typename T>
bool Vector2D<T>::compareVector2D(Vector2D<T> &a, Vector2D<T> &b) {
  return (a.x == b.x) ? a.y < b.y : a.x < b.x;
}

template<Typename T>
bool Vector2D<T>::operator==(const Vector2D<T> &rhs) const {
  return (rhs.x == x) && (rhs.y == y);
}

template<Typename T>
bool Vector2D<T>::operator!=(const Vector2D<T> &rhs) const {
  return !(*this == rhs);
}

template<Typename T>
Vector2D<T> Vector2D<T>::operator-(const Vector2D<T> &rhs) const {
  return Vector2D<T>(x - rhs.x, y - rhs.y);
}

template<Typename T>
Vector2D<T> Vector2D<T>::operator+(const Vector2D<T> &rhs) const {
  return Vector2D<T>(x + rhs.x, y + rhs.y);
}

template<Typename T>
void Vector2D<T>::print() {
  std::cout << "(" << x << ", " << y << ")" << "\n";
}