#include "spaceship.h"

#include <cmath>

#include "game.h"

SpaceShip::SpaceShip(Game* game)
: GameObject(game)
, k_reload_time_(5)
, k_max_velocity_(10.0f)
, reload_countdown_(0)
{
}

void SpaceShip::Update()
{
  UpdatePosition();

  // Spaceships slow down.
  velocity_.x *= .99f;
  velocity_.y *= .99f;

  // Spaceships wrap around the window.
  if (position_.x < 0.0f) {
    position_.x = game_->WindowSize().width;
  } else if (position_.x > game_->WindowSize().width) {
    position_.x = 0.0f;
  }

  if (position_.y < 0.0f) {
    position_.y = game_->WindowSize().height;
  } else if (position_.y > game_->WindowSize().height) {
    position_.y = 0.0f;
  }

  // Countdown until next bullet is available.
  if (reload_countdown_ > 0) {
    --reload_countdown_;
  }
}

bool SpaceShip::Shoot(Bullet* bullet)
{
  if (reload_countdown_ > 0) {
    return false;
  }

  bullet->bounding_box_ = Size(2.0f, 2.0f);
  bullet->position_ = position_;  // Update
  bullet->velocity_.x = velocity_.x + k_max_velocity_ * 1.2 * -sin(direction_ / 360.0f * 2 * M_PI);
  bullet->velocity_.y = velocity_.y + k_max_velocity_ * 1.2 * cos(direction_ / 360.0f * 2 * M_PI);
  bullet->direction_ = direction_;

  reload_countdown_ = k_reload_time_;

  return true;
}

void SpaceShip::Turn(const enum Turn turn)
{
  switch (turn) {
    case Left:
     direction_ += 10.0f;
     break;
    case Right:
     direction_ -= 10.0f;
     break;
  }
}

void SpaceShip::Thrust(const enum Thrust thrust)
{
  switch (thrust) {
    case Forward:
      velocity_.x -= sin(direction_ / 360.0f * 2 * M_PI);
      velocity_.y += cos(direction_ / 360.0f * 2 * M_PI);
      break;
    case Backward:
      velocity_.x += sin(direction_ / 360.0f * 2 * M_PI);
      velocity_.y -= cos(direction_ / 360.0f * 2 * M_PI);
      break;
  }

  // Limit speed.
  if (velocity_.x > k_max_velocity_) {
    velocity_.x = k_max_velocity_;
  } else if (velocity_.x < -k_max_velocity_) {
    velocity_.x = -k_max_velocity_;
  }

  if (velocity_.y > k_max_velocity_) {
    velocity_.y = k_max_velocity_;
  } else if (velocity_.y < -k_max_velocity_) {
    velocity_.y = -k_max_velocity_;
  }
}
