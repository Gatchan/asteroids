#include "point.h"

#include <cmath>

Point::Point()
: x(0.0f)
, y(0.0f)
{
}

Point::Point(const Point& other)
: x(other.x)
, y(other.y)
{
}


Point::Point(float x, float y)
: x(x)
, y(y)
{
}

float Point::Dot(const Point& other) const
{
  return x * other.x + y * other.y;
}

Point Point::operator*(const float scalar) const
{
  return Point(x * scalar, y * scalar);
}

Point Point::operator+(const Point& other) const
{
  return Point(x + other.x, y + other.y);
}

Point Point::operator-(const Point& other) const
{
  return Point(x - other.x, y - other.y);
}

float distance(const Point& lhs, const Point& rhs)
{
  return sqrt(pow(lhs.x - rhs.x, 2) + pow(lhs.y - rhs.y, 2));
}

Point& Point::operator*=(const float scalar)
{
  x *= scalar;
  y *= scalar;
  return *this;
}

Point& Point::operator+=(const Point& other)
{
  x += other.x;
  y += other.y;
  return *this;
}

Point& Point::operator-=(const Point& other)
{
  x -= other.x;
  y -= other.y;
  return *this;
}
