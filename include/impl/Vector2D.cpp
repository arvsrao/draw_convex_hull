#define _USE_MATH_DEFINES

#include <cmath>
#include <iostream>

const double PI = 3.14159265358979323846;

template <typename T>
Vector2D<T>::Vector2D() : x(0), y(0) {}

template <typename T>
Vector2D<T>::Vector2D(const Vector2D<int> &other) {
  x = (T)other.x;
  y = (T)other.y;
}

/*template <typename T>
Vector2D<T>::Vector2D(const Vector2D<int> &&other) {
  x = (T) other.x;
  y = (T) other.y;
}
*/
template <typename T>
Vector2D<T>::Vector2D(T _x, T _y) : x(_x), y(_y) {}

template <typename T>
T Vector2D<T>::dot(const Vector2D<T> &other) const {
  return x * other.x + y * other.y;
}

template <typename T>
double Vector2D<T>::length() const {
  return std::sqrt((double)(x * x + y * y));
}

template <typename T>
double Vector2D<T>::arccos() const {
  // dot with (-1,0)
  if (y < 0) return std::acos((double)x / this->length()) + PI;
  return std::acos(double(-x) / this->length());
}

/**
 *.  Rotate vector by angle theta
 *
 *   [ cos(theta) -sin(theta) ]  * [x]
 *   [ sin(theta)  cos(theta) ]    [y]
 *
 */
template <typename T>
Vector2D<T> Vector2D<T>::rotate(double theta) const {
  double sin_theta = std::sin(theta);
  double cos_theta = std::cos(theta);

  return Vector2D<double>(cos_theta * x - sin_theta * y, sin_theta * x + cos_theta * y);
}

template <typename T>
void Vector2D<T>::normalize() {
  double _length = this->length();
  x              = (T)((double)x / _length);
  y              = (T)((double)y / _length);
}

template <typename T>
bool Vector2D<T>::compareVector2D(const Vector2D<T> &a, const Vector2D<T> &b) {
  return (a.x == b.x) ? a.y < b.y : a.x < b.x;
}

template <typename T>
bool Vector2D<T>::operator==(const Vector2D<T> &rhs) const {
  return (rhs.x == x) && (rhs.y == y);
}

template <typename T>
bool Vector2D<T>::operator>(const Vector2D<T> &rhs) const {
  return (y == rhs.y) ? x < rhs.x : y > rhs.y ;
}

template <typename T>
bool Vector2D<T>::operator>=(const Vector2D<T> &rhs) const {
  return (*this > rhs) || (*this == rhs);
}

template <typename T>
bool Vector2D<T>::operator<(const Vector2D<T> &rhs) const {
  return !(*this >= rhs);
}

template <typename T>
bool Vector2D<T>::operator!=(const Vector2D<T> &rhs) const {
  return !(*this == rhs);
}

template <typename T>
Vector2D<T> Vector2D<T>::operator-(const Vector2D<T> &rhs) const {
  return Vector2D<T>(x - rhs.x, y - rhs.y);
}

template <typename T>
Vector2D<T> Vector2D<T>::operator+(const Vector2D<T> &rhs) const {
  return Vector2D<T>(x + rhs.x, y + rhs.y);
}

template <typename T>
void Vector2D<T>::print() {
  std::cout << "(" << x << ", " << y << ")"
            << "\n";
}

template <typename T>
Vector2D<T>::~Vector2D() {}
