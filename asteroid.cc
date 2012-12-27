#include "asteroid.h"

#include <cmath>
#include <cstdlib>

Asteroid::Asteroid()
: rotation_speed_((rand() / float(RAND_MAX) - .5f) * 6.0f)
{
}
