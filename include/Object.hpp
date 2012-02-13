#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "cstring"
#include "cstdlib"
#include "libtcod.hpp"
#include "Types.hpp"
#include "Zone.hpp"
#include "CSheet.hpp"

class Object {
public:
  Zone* zone;
  Coord loc;
  char c;
  char* name;
  TCODColor color;
  bool blocks;
  CSheet* csheet;

  Object (Zone* zone, const Coord& new_loc, char new_c, const char* new_name, 
          TCODColor new_color, bool new_blocks, CSheet* new_csheet);

  ~Object ();
 
  void move (int dx, int dy);

  void move_to (const Coord& new_loc);
};

#endif
