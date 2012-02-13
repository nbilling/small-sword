#include "Tile.hpp"

Tile::Tile () {
  c = '#';
  blocked = true;
  explored = false;
  block_sight = true;
}

Tile::Tile (char new_c, int new_blocked, int new_block_sight) {
  c = new_c;
  blocked = new_blocked;
  explored = false;
  block_sight = new_block_sight;
}

Tile::~Tile () {}
