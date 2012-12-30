#pragma once

struct Point;
typedef Point Vector;

struct Point
{
  Point();
  Point(const Point& other);
  Point(float x, float y);

  float Dot(const Point& other) const;
  Point operator*(const float scalar) const;
  Point operator+(const Point& other) const;
  Point operator-(const Point& other) const;
  Point& operator*=(const float scalar);
  Point& operator+=(const Point& other);
  Point& operator-=(const Point& other);

  float x;
  float y;
};

float distance(const Point& lhs, const Point& rhs);
