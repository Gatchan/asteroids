#pragma once

#include <vector>

#include "size.h"
#include "gameobject.h"
#include "spaceship.h"
#include "asteroid.h"
#include "bullet.h"

class Game
{
  public:
    Game();

    void SetWindowSize(const Size& size);
    Size WindowSize() const;

    SpaceShip spaceship_;
    std::vector<Bullet> bullets_;
    std::vector<Asteroid> asteroids_;

    bool over_;

  private:
    Size window_;
};
