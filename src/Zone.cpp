#include "Zone.hpp"

using namespace std;

Zone::Zone (int new_grid_w, int new_grid_h){
  /*Creates a new zone.
    - 'grid' filled with tiles using default tile constructor.
    - 'objects' list empty.*/
  grid = (Tile::Tile***) malloc (sizeof (Tile::Tile**) * new_grid_w);
  for (int x=0; x < new_grid_w; x++){
    grid[x] = (Tile**) malloc (sizeof (Tile::Tile*) * new_grid_h);
    for (int y=0; y < new_grid_h; y++){
      grid[x][y] = new Tile::Tile();
    }
  }
  grid_w = new_grid_w;
  grid_h = new_grid_h;
  objects = new list<Object*>();
}

Zone::~Zone () {
  /*WARNING: This destructor will PURGE all memory pointed to by the Zone.
    If you want to keep some Objects then copy their pointers and remove
    them from 'objects' list in this zone FIRST.*/
  
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
  //Delete all objects in 'objects' list
  for (list<Object*>::iterator o = objects->begin(); o != objects->end(); o++){
    delete (*o);
  }
  //Delete objects list
  delete objects;
}

bool Zone::is_blocked (const Coord& loc) {
  if (grid[loc.x][loc.y]->blocked){
    return true;
  }
  for (list<Object*>::iterator o = objects->begin(); o != objects->end(); o++){
    if ((*o)->blocks && (*o)->loc.x == loc.x && (*o)->loc.y == loc.y){
      return (true);
    }     
  }
  return (false);
}

bool Zone::in_bounds (const Coord& loc) {
  return (0 <= loc.x && loc.x < grid_w && 0 <= loc.y && loc.y < grid_h);
}
