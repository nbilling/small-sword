#ifndef TILE_HPP
#define TILE_HPP

class Tile {
public:
  char c;
  int blocked;
  int explored;
  int block_sight;
  
  Tile ();

  Tile (char new_c, int new_blocked, int new_block_sight);

  ~Tile();

};

#endif
