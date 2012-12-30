#include "utils.h"

#include <cmath>

Vector directionToVector(const float direction)
{
  const float factor = 1.0f / 360.0f * 2.0f * M_PI;
  return Vector(-sin(direction * factor), cos(direction * factor));
}
