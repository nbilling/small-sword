#include "Zone.hpp"

using namespace std;

Zone::Zone (int new_grid_w, int new_grid_h, map<int,Object*>* new_object_registry){
  object_locations = new map<int,Coord> ();
  object_registry = new_object_registry;

  grid = (Tile::Tile***) malloc (sizeof (Tile::Tile**) * new_grid_w);
  for (int x=0; x < new_grid_w; x++){
    grid[x] = (Tile**) malloc (sizeof (Tile::Tile*) * new_grid_h);
    for (int y=0; y < new_grid_h; y++){
      grid[x][y] = new Tile::Tile();
    }
  }
  grid_w = new_grid_w;
  grid_h = new_grid_h;

  blocked = new bool*[new_grid_w];
  for (int i=0; i < new_grid_w; i++) {
    blocked[i] = new bool[new_grid_h];
    for (int j=0; j < new_grid_h; j++)
      blocked[i][j] = grid[i][j]->blocked;
  }
}

Zone::~Zone () {
  delete (object_locations);
  // Note: does not deallocate the object_registry.
  for (int i=0; i < grid_w; i++)
    delete (blocked[i]);
  delete (blocked);
  
  for (int x=0; x < grid_w; x++) {
    //Delete all tiles in this row (Tile::Tile*)
    for (int y=0; y < grid_h; y++){
      delete (grid[x][y]);
    }
    //Delete pointer to this row (Tile::Tile**)
    free (grid[x]);
  }
  //Delete base of row pointers (Tile::Tile***)
  free (grid);
  //Delete objects list
}

// Place object given by object_id at loc. Do nothing if loc is blocked.
// If you call this on an object already in the zone then it will break
// the zone.
void Zone::place_object (int object_id,  const Coord& loc) {
  if (blocked[loc.x][loc.y])
    return;
  (*object_locations)[object_id] = loc;
  blocked[loc.x][loc.y] = grid[loc.x][loc.y]->blocked
    || ((*object_registry)[object_id])->blocks;
}

// Move object given by object_id to loc. Do nothing if loc is blocked.
// If you call this on an object not already in the zone then it will
// break the zone.
void Zone::move_object (int object_id,  const Coord& loc) {
  if (blocked[loc.x][loc.y])
    return;
  Coord old_loc = (*object_locations)[object_id];
  blocked[old_loc.x][old_loc.y] = grid[old_loc.x][old_loc.y]->blocked;
  (*object_locations)[object_id] = loc;
  blocked[loc.x][loc.y] = grid[loc.x][loc.y]->blocked
    || ((*object_registry)[object_id])->blocks;
}

bool Zone::is_blocked (const Coord& loc) {
  return (blocked[loc.x][loc.y]);
}

bool Zone::in_bounds (const Coord& loc) {
  return (0 <= loc.x && loc.x < grid_w && 0 <= loc.y && loc.y < grid_h);
}

list<int>* Zone::objects_at (const Coord& loc) {
  list<int>* retval = new list<int> ();
  for (map<int,Coord>::iterator it = object_locations->begin ();
       it != object_locations->end (); it++) {
    if (coord_eq ((*it).second, loc)) {
      retval->push_front ((*it).first);
    }
  }
  return (retval);
}

Coord Zone::location_of (int object_id) {
  return ((*object_locations)[object_id]);
}
