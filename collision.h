#pragma once

#include "gameobject.h"

#include <vector>

bool collision(const GameObject& lhs, const GameObject& rhs);
void computeBoundingBox(const GameObject& object, std::vector<Point>* points);
