#ifndef FORMULAS_HPP
#define FORMULAS_HPP

#include "cmath"
#include "Types.hpp"
#include "Object.hpp"

int distance_to (Object* src_object, Object* dest_object);
int distance_to (Object* src_object, const Coord& dest);
int distance_to (const Coord& src, const Coord& dest);
int displacement_to_direction (int dx, int dy);

#endif
