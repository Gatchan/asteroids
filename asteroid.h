#pragma once

#include "gameobject.h"

class Asteroid: public GameObject
{
  public:
    Asteroid(Game* game);

    float rotation_speed_;

};
