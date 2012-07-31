#ifndef FORMULAS_HPP
#define FORMULAS_HPP

#include "cmath"

#include "Object.hpp"
#include "Types.hpp"

#define min(x, y) ((x > y)?(y):(x))
#define max(x, y) ((x > y)?(x):(y))

int distance_to (const Coord& src, const Coord& dest);
int displacement_to_direction (int dx, int dy);

#endif
