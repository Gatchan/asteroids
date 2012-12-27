#include "collision.h"

#include <iostream>
#include <cmath>

void translateBoundingBox(const GameObject& object, std::vector<Point>* points)
{
  const Point& position = object.position_;
  const BoundingBox& bounding_box = object.bounding_box_;

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
  return Point(cos(angle) * (point.x - origin.x) - \
               sin(angle) * (point.y - origin.y) + origin.x,
               sin(angle) * (point.x - origin.x) + \
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
  rotateBoundingBox(object.position_, object.direction_ / 360 * 2 * M_PI,
                    points);
}

bool collision(const GameObject& lhs, const GameObject rhs)
{
  std::vector<Point> lhs_points;
  translateBoundingBox(lhs, &lhs_points);
  rotateBoundingBox(lhs.position_, lhs.direction_ / 360 * 2 * M_PI, &lhs_points);

  std::vector<Point> rhs_points;
  translateBoundingBox(rhs, &rhs_points);
  rotateBoundingBox(rhs.position_, rhs.direction_ / 360 * 2 * M_PI, &rhs_points);

  return false;
}
