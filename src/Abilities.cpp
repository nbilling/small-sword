#include "Abilities.hpp"
#include "iostream"

void walk (Object* actor, int direction) {
  Coord new_loc = actor->loc;

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

  if (actor->zone->in_bounds (new_loc)) {
    if (!(actor->zone->is_blocked (new_loc))){
      actor->move_to (new_loc);
    }
  }
  else
    cerr << "Abilities => walk => Attempt to walk out of bounds." << endl;
}

void walk (Object* actor, int dx, int dy) {
  Coord new_loc;
  new_loc.x = actor->loc.x + dx;
  new_loc.y = actor->loc.y + dy;

  if (actor->zone->in_bounds (new_loc)) {
    if (!(actor->zone->is_blocked (new_loc))){
      actor->move_to (new_loc);
    }
  }
  else
    cerr << "Abilities => walk => Attempt to walk out of bounds." << endl;
}

void attack (Object* src_object, Object* dest_object) {
  if ((src_object->zone == dest_object->zone) 
      && distance_to (src_object, dest_object) < 2) {
    dest_object->csheet->hp -=1;
  }
}
