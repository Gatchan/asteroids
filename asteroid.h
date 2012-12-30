#pragma once

#include "gameobject.h"

class Asteroid: public GameObject
{
  public:
    Asteroid(Game* game);
    void Update();

    float rotation_speed_;

};
