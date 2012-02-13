#ifndef ZONE_HPP
#define ZONE_HPP

#include "cmath"
#include "list"
#include "Types.hpp"
#include "Tile.hpp"
#include "Object.hpp"

using namespace std;

class Zone{
public:
  //List of objects in Zone
  list<Object*>* objects;
  //Two-dimensional array of Tiles
  Tile::Tile*** grid;

  //Grid dimensions
  int grid_w;
  int grid_h;

  Zone (int new_grid_w, int new_grid_h);

  ~Zone ();

  bool is_blocked (const Coord& loc);

  bool in_bounds (const Coord& loc);
};

#endif
