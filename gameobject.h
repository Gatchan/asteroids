#pragma once

#include "point.h"
#include "boundingbox.h"

typedef Point Vector;

class GameObject
{
  public:
    Point position_;
    Vector velocity_;
    float direction_;

    BoundingBox bounding_box_;
};
