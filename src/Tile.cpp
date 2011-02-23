class Tile {
public:
  char c;
  int blocked;
  int explored;
  int block_sight;

  Tile () {
    c = '#';
    blocked = true;
    explored = false;
    block_sight = true;
  }

  Tile (char new_c, int new_blocked, int new_block_sight) {
    c = new_c;
    blocked = new_blocked;
    explored = false;
    block_sight = new_block_sight;
  }

  ~Tile () {}

};
  
