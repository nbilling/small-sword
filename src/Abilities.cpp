#include "Abilities.hpp"
#include "iostream"

void walk (int obj_id, Zone* zone, list<Coord>* path) {
  for (list<Coord>::iterator it = path->begin (); 
       it != path->end (); it++) {
    if (!zone->in_bounds (*it)) {
      cerr << "Abilities => walk => attempt to walk out of bounds" << endl;
      break;
    }
    if (zone->is_blocked (*it))
      break;
    zone->move_object (obj_id, *it);
  }
}

void step (int obj_id, Zone* zone, int direction) {
  list<Coord>* path = new list<Coord> ();
  Coord new_loc = zone->location_of (obj_id);
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
  path->push_front (new_loc);
  walk (obj_id, zone, path);
}

void attack (Object* src_object, Object* dest_object) {
  dest_object->csheet->hp -=1;
}
