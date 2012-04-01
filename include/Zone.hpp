#ifndef ZONE_HPP
#define ZONE_HPP

#include "cmath"
#include "list"
#include "map"
#include "Types.hpp"
#include "Tile.hpp"
#include "Object.hpp"
using namespace std;

#define color_wall TCODColor::grey
#define color_ground TCODColor::desaturatedOrange

class Zone{
public:
  // Inhabitants
  map<int,Coord>* object_locations;

  // Bookkeeping
  bool** blocked;

  // Terrain
  Tile::Tile*** grid;

  // Grid dimensions
  int grid_w;
  int grid_h;

  Zone (int new_grid_w, int new_grid_h);

  ~Zone ();

  void place_object (int object_id, const Coord& loc);

  void move_object (int object_id, const Coord& loc);

  bool is_blocked (const Coord& loc);

  bool in_bounds (const Coord& loc);

  list<int>* objects_at (const Coord& loc);

  Coord location_of (int object_id);
};

#endif
