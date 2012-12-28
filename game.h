#pragma once

#include <vector>

#include "gameobject.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"

class Game
{
  public:
    Game();

    SpaceShip spaceship_;
    std::vector<Bullet> bullets_;
    std::vector<Asteroid> asteroids_;

    bool over_;
    float panel_width_;
    float panel_height_;
};
