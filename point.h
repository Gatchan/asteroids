#pragma once

struct Point
{
  Point();
  Point(float x, float y);

  float operator*(const Point& other) const;  // Dot product.
  Point operator+(const Point& other) const;
  Point operator-(const Point& other) const;

  float x;
  float y;
};

float distance(const Point& lhs, const Point& rhs);
