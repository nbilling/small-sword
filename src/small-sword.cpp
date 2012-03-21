//Standard library
#include "iostream"
#include "list"
using namespace std;

//libtcod
#include "libtcod.hpp"

//My files
#include "AI.hpp"
#include "TacticalUI.hpp"
#include "Zone.hpp"
#include "DungeonGenerator.hpp"

//#include "TacticalUI.cpp"

//Definitions
#define ROOT_CONSOLE_WIDTH 100
#define ROOT_CONSOLE_HEIGHT 50

#define MAP_CONSOLE_WIDTH 80
#define MAP_CONSOLE_HEIGHT 50

#define HUD_CONSOLE_WIDTH 20
#define HUD_CONSOLE_HEIGHT 50
 
#define GRID_WIDTH 80
#define GRID_HEIGHT 45 
 
#define LIMIT_FPS 60 


void find_empty_tile (Zone* zone, Object* player) {
  //Put player in first unblocked tile found
  for (int x=0; x < GRID_WIDTH; x++) {
    for (int y=0; y< GRID_HEIGHT; y++) {
      if (!zone->is_blocked ((Coord){x, y})) {
        zone->place_object(player->id, (Coord){x, y});
        return;
      }
    }
  }
}

void delete_registered_objects (map<int,Object*>* object_registry) {
  for (map<int,Object*>::iterator m = object_registry->begin (); 
       m != object_registry->end (); m++)
    delete ((*m).second);
}

int main (int argc, const char** argv) {
  TCODConsole::setCustomFont ("data/arial10x10.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD);
  cerr << "+ Init root console" << endl;
  TCODConsole::initRoot (ROOT_CONSOLE_WIDTH, ROOT_CONSOLE_HEIGHT, "python/libtcod tutorial", false);
  TCODSystem::setFps (LIMIT_FPS);

  cerr << "+ Create offscreen map console" << endl;
  TCODConsole* map_console = new TCODConsole::TCODConsole (MAP_CONSOLE_WIDTH, MAP_CONSOLE_HEIGHT);  

  cerr << "+ Create offscreen hud console" << endl;
  TCODConsole* hud_console = new TCODConsole::TCODConsole (HUD_CONSOLE_WIDTH, HUD_CONSOLE_HEIGHT);  

  cerr << "+ Create Object registry" << endl;
  map<int,Object*>* object_registry = new map<int,Object*> ();

  cerr << "+ Create zone" << endl;
  Zone* zone = new Zone (GRID_WIDTH, GRID_HEIGHT, object_registry);

  cerr << "+ Create AI list" << endl;
  list<AI*>* ais = new list<AI*>();

  cerr << "+ Generate dungeon" << endl;
  make_grid (zone, object_registry, ais);
  
  cerr << "+ Create player" << endl;
  Object* player = new Object ('@', "player", TCODColor::white, true, new CSheet(20));
  (*object_registry)[player->id] = player;

  cerr << "+ Place player" << endl;
  find_empty_tile (zone, player);

  cerr << "+ Set up camera fov_map" << endl;
  TCODMap* fov_map = new TCODMap::TCODMap (GRID_WIDTH, GRID_HEIGHT);
  for (int y=0; y < GRID_HEIGHT; y++)
    for (int x=0; x < GRID_WIDTH; x++) {
      fov_map->setProperties (x, y, !zone->grid[x][y]->blocked, 
                              !zone->grid[x][y]->block_sight);
    }

  cerr << "+ Create TacticalUI" << endl;
  TacticalUI* tactical = new TacticalUI (zone, ais, player, fov_map, 
                                         map_console, MAP_CONSOLE_WIDTH, 
                                         MAP_CONSOLE_HEIGHT, hud_console, 
                                         HUD_CONSOLE_WIDTH, HUD_CONSOLE_HEIGHT);

  cerr << "+ Initialisation complete" << endl;

  tactical->display ();

  cerr << "+ Window closed" << endl;

  delete tactical;
  delete TCODConsole::root;
  delete map_console;
  delete hud_console;
  delete fov_map;
  delete zone;
  delete_registered_objects (object_registry);
  delete (object_registry);
  cerr << "+ Memory deallocation completed" << endl;

  return(0);
}
