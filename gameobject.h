#pragma once

#include "point.h"
#include "boundingbox.h"
#include "utils.h"

class Game;

class GameObject
{
  public:
    GameObject(Game* game);

    virtual void Update() = 0;

    bool OnScreen() const;

    Point Position() const;
    float Direction() const;
    Vector Velocity() const;
    BoundingBox Bounding() const;

    void SetPosition(const Point& position);
    void SetDirection(const float direction);
    void SetVelocity(const Vector& velocity);
    void SetBounding(const BoundingBox& bounding_box);

  protected:
    // Simply update the position using the velocity of the object.
    void UpdatePosition();

    Point position_;
    Vector velocity_;
    float direction_;
    BoundingBox bounding_box_;

    Game* game_;
};
