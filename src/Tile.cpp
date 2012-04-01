#include "Tile.hpp"

Tile::Tile (char new_c, TCODColor new_color, int new_blocked, int new_block_sight) {
  c = new_c;
  color = new_color;
  blocked = new_blocked;
  explored = false;
  block_sight = new_block_sight;
}

Tile::~Tile () {}
