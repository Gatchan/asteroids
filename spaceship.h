#pragma once

#include "gameobject.h"
#include "bullet.h"

class Game;

class SpaceShip: public GameObject
{
  public:
    SpaceShip(Game* game);
    bool Shoot(Bullet* bullet);

    void Update();  // This is so gonna be a virtual method...

  private:
    const float k_reload_time_;
    float k_max_velocity_;
    float reload_countdown_;
};
