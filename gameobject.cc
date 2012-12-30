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

