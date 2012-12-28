#include "point.h"

#include <cmath>

Point::Point()
: x(0.0f)
, y(0.0f)
{}

Point::Point(float x, float y)
: x(x)
, y(y)
{}

float Point::operator*(const Point& other) const
{
  return x * other.x + y * other.y;
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
