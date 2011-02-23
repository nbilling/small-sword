//Standard library
#include "cstring"
#include "cstdlib"

//libtcod
#include "libtcod.hpp"

class Object {
public:
  int x;
  int y;
  char c;
  char* name;
  TCODColor color;
  bool blocks;

  Object (int new_x, int new_y, char new_c, const char* new_name, 
          TCODColor new_color, bool new_blocks) {
    x = new_x;
    y = new_y;
    c = new_c;
    name = (char*) malloc (1 + sizeof (char) * strlen (new_name));
    strcpy (name, new_name);
    color = new_color;
    blocks = new_blocks;
  }

  ~Object () {
    free (name);
  }
 
  void move (int dx, int dy) {
    x += dx;
    y += dy;
  }
};

