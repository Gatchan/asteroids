#include "bullet.h"

Bullet::Bullet(Game* game)
: GameObject(game)
{
}

void Bullet::Update()
{
  UpdatePosition();
}
