#ifndef TILE_HPP
#define TILE_HPP

#include "libtcod.hpp"

class Tile {
public:
  char c;
  TCODColor color;
  int blocked;
  int explored;
  int block_sight;

  Tile (char new_c, TCODColor new_color, int new_blocked, int new_block_sight);

  ~Tile();

};

#endif
