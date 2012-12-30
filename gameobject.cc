#include "gameobject.h"
#include "game.h"

GameObject::GameObject(Game* game)
: game_(game)
{
}

void GameObject::UpdatePosition()
{
  position_.x = position_.x + velocity_.x;
  position_.y = position_.y + velocity_.y;
}


bool GameObject::OnScreen() const {
  if (position_.x < 0.0f || position_.x > game_->WindowSize().width ||
      position_.y < 0.0f || position_.y > game_->WindowSize().height) {
    return false;
  }

  return true;
}

Point GameObject::Position() const
{
  return position_;
}

float GameObject::Direction() const
{
  return direction_;
}

Vector GameObject::Velocity() const
{
  return velocity_;
}

BoundingBox GameObject::Bounding() const
{
  return bounding_box_;
}

void GameObject::SetPosition(const Point& position)
{
  position_ = position;
}

void GameObject::SetDirection(const float direction)
{
  direction_ = direction;
}

void GameObject::SetVelocity(const Vector& velocity)
{
  velocity_ = velocity;
}

void GameObject::SetBounding(const BoundingBox& bounding_box)
{
  bounding_box_ = bounding_box;
}
