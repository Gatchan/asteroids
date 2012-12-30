#include "collision.h"

#include <iostream>
#include <cmath>
#include <cassert>

void translateBoundingBox(const GameObject& object, std::vector<Point>* points)
{
  const Point& position = object.Position();
  const BoundingBox& bounding_box = object.Bounding();

  points->push_back(Point(position.x + bounding_box.width / 2.0f,
                          position.y + bounding_box.height / 2.0f));
  points->push_back(Point(position.x - bounding_box.width / 2.0f,
                          position.y + bounding_box.height / 2.0f));
  points->push_back(Point(position.x - bounding_box.width / 2.0f,
                          position.y - bounding_box.height / 2.0f));
  points->push_back(Point(position.x + bounding_box.width / 2.0f,
                          position.y - bounding_box.height / 2.0f));
}

Point rotatePoint(const Point& origin, const Point& point, const float angle)
{
  return Point(cos(angle) * (point.x - origin.x) -
               sin(angle) * (point.y - origin.y) + origin.x,
               sin(angle) * (point.x - origin.x) +
               cos(angle) * (point.y - origin.y) + origin.y);
}

void rotateBoundingBox(const Point& origin, const float angle,
                       std::vector<Point>* points)
{
  for (size_t i = 0; i < points->size(); ++i) {
    (*points)[i] = rotatePoint(origin, (*points)[i], angle);
  }
}

void computeBoundingBox(const GameObject& object, std::vector<Point>* points)
{
  translateBoundingBox(object, points);
  rotateBoundingBox(object.Position(), object.Direction() / 360 * 2 * M_PI,
                    points);
}

void project(const Point& axis, const std::vector<Point>& shape,
             float* minimum, float* maximum)
{
  assert(shape.size() > 0);
  *minimum = *maximum = axis.Dot(shape[0]);

  for (size_t i = 1; i < shape.size(); ++i) {
    const float projection = axis.Dot(shape[i]);
    *minimum = std::min(*minimum, projection);
    *maximum = std::max(*maximum, projection);
  }
}

bool overlap(const std::vector<Point>& lhs, const std::vector<Point>& rhs)
{
  for (size_t i = 0; i < lhs.size(); ++i) {
    const Point edge = lhs[(i + 1) % lhs.size()] - lhs[i];
    const Point perpendicular(-edge.y, edge.x);

    float min_lhs, max_lhs;
    project(perpendicular, lhs, &min_lhs, &max_lhs);

    float min_rhs, max_rhs;
    project(perpendicular, rhs, &min_rhs, &max_rhs);

    if (min_lhs > max_rhs || max_lhs < min_rhs) {
      return false;
    }
  }

  return true;
}

bool collision(const GameObject& lhs, const GameObject& rhs)
{
  std::vector<Point> lhs_points;
  translateBoundingBox(lhs, &lhs_points);
  rotateBoundingBox(lhs.Position(), lhs.Direction() / 360 * 2 * M_PI,
                    &lhs_points);

  std::vector<Point> rhs_points;
  translateBoundingBox(rhs, &rhs_points);
  rotateBoundingBox(rhs.Position(), rhs.Direction() / 360 * 2 * M_PI,
                    &rhs_points);

  if (!overlap(lhs_points, rhs_points) || !overlap(rhs_points, lhs_points)) {
    return false;
  }

  return true;
}

