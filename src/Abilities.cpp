#include "Abilities.hpp"
#include "iostream"

void walk (Object* actor, Zone* zone, int direction) {
  Coord new_loc = (*(zone->object_locations))[actor->id];

  switch (direction) {
    case 1:
      new_loc.x += -1;
      new_loc.y += 1;
      break;
    case 2:
      new_loc.x += 0;
      new_loc.y += 1;
      break;
    case 3:
      new_loc.x += 1;
      new_loc.y += 1;
      break;
    case 4:
      new_loc.x += -1;
      new_loc.y += 0;
      break;
    case 5:
      new_loc.x += 0;
      new_loc.y += 0;
      break;
    case 6:
      new_loc.x += 1;
      new_loc.y += 0;
      break;
    case 7:
      new_loc.x += -1;
      new_loc.y += -1;
      break;
    case 8:
      new_loc.x += 0;
      new_loc.y += -1;
      break;
    case 9:
      new_loc.x += 1;
      new_loc.y += -1;
      break;
    }

  if (zone->in_bounds (new_loc)) {
    if (!(zone->is_blocked (new_loc))){
      zone->move_object (actor->id, new_loc);
    }
  }
  else
    cerr << "Abilities => walk => Attempt to walk out of bounds." << endl;
}

void walk (Object* actor, Zone* zone, int dx, int dy) {
  Coord new_loc = (*(zone->object_locations))[actor->id];
  new_loc.x += dx;
  new_loc.y += dy;

  if (zone->in_bounds (new_loc)) {
    if (!(zone->is_blocked (new_loc))){
      zone->move_object (actor->id, new_loc);
    }
  }
  else
    cerr << "Abilities => walk => Attempt to walk out of bounds." << endl;
}

void attack (Object* src_object, Object* dest_object) {
  dest_object->csheet->hp -=1;
}
