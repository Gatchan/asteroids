#pragma once

#include "gameobject.h"
#include "bullet.h"
#include "enums.h"

class Game;

class SpaceShip: public GameObject
{
  public:
    SpaceShip(Game* game);
    bool Shoot(Bullet* bullet);

    void Update();
    void Thrust(const enum Thrust thrust);
    void Turn(const enum Turn turn);

  private:
    const float reload_time_;
    float max_velocity_;
    float reload_countdown_;
};
