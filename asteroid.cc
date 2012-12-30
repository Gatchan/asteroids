#include <cmath>
#include <cstdlib>

#include "asteroid.h"
#include "game.h"

Asteroid::Asteroid(Game* game)
: GameObject(game)
, rotation_speed_((rand() / float(RAND_MAX) - .5f) * 6.0f)
{
}

void Asteroid::Update()
{
  UpdatePosition();

  direction_ += rotation_speed_;

  // Asteroids bounce on the edges of the window.
  if (position_.x < 0.0f || position_.x > game_->WindowSize().width) {
    velocity_.x *= -1.0f;
  }

  if (position_.y < 0.0f || position_.y > game_->WindowSize().height) {
    velocity_.y *= -1.0f;
  }
}
