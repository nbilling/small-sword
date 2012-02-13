#include "Object.hpp"

Object::Object (Zone* new_zone, const Coord& new_loc, char new_c, const char* new_name, 
                TCODColor new_color, bool new_blocks, CSheet* new_csheet) {
  zone = new_zone;
  loc = new_loc;
  c = new_c;
  name = (char*) malloc (1 + sizeof (char) * strlen (new_name));
  strcpy (name, new_name);
  color = new_color;
  blocks = new_blocks;
  csheet = new_csheet;
}

Object::~Object () {
  free (name);
}
 
void Object::move (int dx, int dy) {
  loc.x += dx;
  loc.y += dy;
}

void Object:: move_to (const Coord& new_loc) {
  loc = new_loc;
}
