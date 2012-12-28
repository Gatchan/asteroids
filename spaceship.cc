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
  if (reload_countdown_ > 0) {
    --reload_countdown_;
  }

  // Update position.
  position_.x = position_.x + velocity_.x;
  position_.y = position_.y + velocity_.y;
  velocity_.x *= .99f;
  velocity_.y *= .99f;

  if (position_.x < 0.0f) {
    position_.x = game_->panel_width_;
  } else if (position_.x > game_->panel_width_) {
    position_.x = 0.0f;
  }

  if (position_.y < 0.0f) {
    position_.y = game_->panel_height_;
  } else if (position_.y > game_->panel_height_) {
    position_.y = 0.0f;
  }

}

bool SpaceShip::Shoot(Bullet* bullet)
{
  if (reload_countdown_ > 0) {
    return false;
  }

  bullet->bounding_box_.width = 2;
  bullet->bounding_box_.height = 2;
  bullet->position_.x = position_.x;  // Update
  bullet->position_.y = position_.y;
  bullet->velocity_.x = velocity_.x + k_max_velocity_ * 1.2 * -sin(direction_ / 360.0f * 2 * M_PI);
  bullet->velocity_.y = velocity_.y + k_max_velocity_ * 1.2 * cos(direction_ / 360.0f * 2 * M_PI);
  bullet->direction_ = direction_;

  reload_countdown_ = k_reload_time_;

  return true;
}
