#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include "Types.hpp"
#include "Formulas.hpp"
#include "Object.hpp"

typedef struct {
  list<Coord>* path;
} AbilityWalk;

typedef struct {
  int direction;
} AbilityStep;

enum AbilityType {
  Walk, Step
};

typedef struct {
  enum AbilityType type;
  union {
    AbilityWalk walk_data;
    AbilityStep step_data;
  } ability_data;
} AbilityInvocation;

void walk (int obj_id, Zone* zone, list<Coord>* path);

void step (int obj_id, Zone* zone, int direction);      

void attack (Object* src_object, Object* dest_object);

#endif
