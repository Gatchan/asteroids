#include "game.h"

#include <iostream>
#include <cstdlib>
#include <cmath>

Game::Game(const Size& window_size)
: spaceship_(this)
, over_(false)
, window_(window_size)
{
  spaceship_.SetPosition(Point(100.0f, 100.0f));
  spaceship_.SetVelocity(Vector(0.0f, 0.0f));
  spaceship_.SetDirection(0.0f);
  spaceship_.SetBounding(BoundingBox(10.0f, 20.0f));

  for (int i = 0; i < 20; ++i) {
    Asteroid a(this);
    a.SetPosition(Point(rand() % int(window_.width),
                        rand() % int(window_.height)));
    a.SetVelocity(Vector((rand() / float(RAND_MAX) - .5f) * 2.0f,
                         (rand() / float(RAND_MAX) - .5f) * 2.0f));
    a.SetDirection(rand() / float(RAND_MAX) * 2 * M_PI);  // XXX
    a.SetBounding(BoundingBox(15.0f + (rand() / float(RAND_MAX) * 10.0f),
                              15.0f + (rand() / float(RAND_MAX) * 10.0f)));

    asteroids_.push_back(a);
  }
}

Size Game::WindowSize() const
{
  return window_;
}
