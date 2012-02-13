#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include "Types.hpp"
#include "Formulas.hpp"
#include "Object.hpp"

void walk (Object* actor, int direction);

void walk (Object* actor, int dx, int dy);

void attack (Object* src_object, Object* dest_object);

#endif
