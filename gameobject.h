#pragma once

#include "point.h"
#include "boundingbox.h"

class Game;

typedef Point Vector;

class GameObject
{
  public:
    GameObject(Game* game);

    virtual void Update() = 0;

    bool OnScreen() const;

    Point position_;
    Vector velocity_;
    float direction_;

    BoundingBox bounding_box_;

  protected:
    // Simply update the position using the velocity of the object.
    void UpdatePosition();

    Game* game_;
};
