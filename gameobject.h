#pragma once

#include "point.h"
#include "boundingbox.h"

class Game;

typedef Point Vector;

class GameObject
{
  public:
    GameObject(Game* game);

    Point position_;
    Vector velocity_;
    float direction_;

    BoundingBox bounding_box_;

  protected:
    Game* game_;
};
