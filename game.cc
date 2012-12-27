#include "game.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

Game::Game()
{
  spaceship_.position_.x = 100.0f;
  spaceship_.position_.y = 100.0f;
  spaceship_.velocity_.x = 0.0f;
  spaceship_.velocity_.y = 0.0f;
  spaceship_.direction_ = 0.0f;
  spaceship_.bounding_box_.width = 10;
  spaceship_.bounding_box_.height = 20;

  for (int i = 0; i < 20; ++i) {
    Asteroid a;
    a.position_.x = rand() % 800;
    a.position_.y = rand() % 600;
    a.velocity_.x = (rand() / float(RAND_MAX) - .5f) * 2.0f;
    a.velocity_.y = (rand() / float(RAND_MAX) - .5f) * 2.0f;
    a.direction_ = rand() / float(RAND_MAX) * 2 * M_PI;
    a.bounding_box_.width = 15 + (rand() / float(RAND_MAX) * 10.0f);
    a.bounding_box_.height = 15 + (rand() / float(RAND_MAX) * 10.0f);
    asteroids_.push_back(a);
  }
}
