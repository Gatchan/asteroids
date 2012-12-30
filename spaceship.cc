#include "spaceship.h"

#include <cmath>

#include "game.h"
#include "utils.h"

SpaceShip::SpaceShip(Game* game)
: GameObject(game)
, reload_time_(5)
, max_velocity_(10.0f)
, reload_countdown_(0)
{
}

void SpaceShip::Update()
{
  UpdatePosition();

  // Spaceships slow down.
  velocity_ *= .99f;

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

  reload_countdown_ = reload_time_;

  bullet->SetBounding(Size(2.0f, 2.0f));
  bullet->SetPosition(position_);  // Update
  bullet->SetDirection(direction_);
  bullet->SetVelocity(directionToVector(direction_) * max_velocity_ * 1.2f +
                      velocity_);

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
      velocity_ += directionToVector(direction_);
      break;
    case Backward:
      velocity_ -= directionToVector(direction_);
      break;
  }

  // Limit speed.
  if (velocity_.x > max_velocity_) {
    velocity_.x = max_velocity_;
  } else if (velocity_.x < -max_velocity_) {
    velocity_.x = -max_velocity_;
  }

  if (velocity_.y > max_velocity_) {
    velocity_.y = max_velocity_;
  } else if (velocity_.y < -max_velocity_) {
    velocity_.y = -max_velocity_;
  }
}
