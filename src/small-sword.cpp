//Standard library
#include "iostream"
#include "list"
using namespace std;

//libtcod
#include "libtcod.hpp"

//My files
#include "TacticalUI.cpp"

//Definitions
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 50
 
#define GRID_WIDTH 80
#define GRID_HEIGHT 45 
 
#define LIMIT_FPS 60 

inline
void find_empty_tile (Zone* zone, Object* player) {
  //Put player in first unblocked tile found
  for (int x=0; x < GRID_WIDTH; x++) {
    for (int y=0; y< GRID_HEIGHT; y++) {
      if (!zone->is_blocked (x, y)) {
        player->x = x;
        player->y = y;
        return;
      }
    }
  }
}

int main (int argc, const char** argv) {
  TCODConsole::setCustomFont ("arial10x10.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD);
  cerr << "+ Init root console" << endl;
  TCODConsole::initRoot (SCREEN_WIDTH, SCREEN_HEIGHT, "python/libtcod tutorial", false);
  TCODSystem::setFps (LIMIT_FPS);

  cerr << "+ Create offscreen console" << endl;
  TCODConsole* con = new TCODConsole::TCODConsole (SCREEN_WIDTH, SCREEN_HEIGHT);  

  cerr << "+ Create zone" << endl;
  Zone* zone = new Zone (GRID_WIDTH, GRID_HEIGHT);

  cerr << "+ Create AI list" << endl;
  list<AI*>* ais = new list<AI*>();

  cerr << "+ Generate dungeon" << endl;
  make_grid (zone, ais);
  
  cerr << "+ Create player" << endl;
  Object* player = new Object (0, 0, '@', "player", TCODColor::white, true, new CSheet(20));

  cerr << "+ Place player" << endl;
  find_empty_tile (zone, player);
  zone->objects->push_back (player);    

  TCODMap* fov_map = new TCODMap::TCODMap (GRID_WIDTH, GRID_HEIGHT);
  for (int y=0; y < GRID_HEIGHT; y++)
    for (int x=0; x < GRID_WIDTH; x++) {
      fov_map->setProperties (x, y, !zone->grid[x][y]->blocked, !zone->grid[x][y]->block_sight);
    }

  TacticalUI* tactical = new TacticalUI (zone, ais, player, fov_map, con, 
                                         SCREEN_WIDTH, SCREEN_HEIGHT);
  tactical->display ();

  cerr << "+ Window closed" << endl;

  delete tactical;
  delete TCODConsole::root;
  delete con;
  delete fov_map;
  delete zone;
  cerr << "+ Memory deallocation completed" << endl;

  return(0);
}
