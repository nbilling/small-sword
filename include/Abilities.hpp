#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include "Types.hpp"
#include "Formulas.hpp"
#include "Object.hpp"

void walk (int obj_id, Zone* zone, list<Coord>* path);

void step (int obj_id, Zone* zone, int direction);      

void attack (Object* src_object, Object* dest_object);

#endif
