//Standard library
#include "iostream"
using namespace std;

//libtcod
#include "libtcod.hpp"

//My files
#include "DungeonGenerator.cpp"

//Definitions
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 50
 
#define GRID_WIDTH 80
#define GRID_HEIGHT 45 
 
#define FOV_ALGO 0  
#define FOV_LIGHT_WALLS true
#define TORCH_RADIUS 10
 
#define LIMIT_FPS 60  
 
#define color_dark_wall TCODColor::darkGrey
#define color_light_wall TCODColor::lightGrey
#define color_dark_ground TCODColor::darkOrange
#define color_light_ground TCODColor::desaturatedOrange

#define GS_PLAYING 1

#define PA_NONE 0

TCODMap* fov_map;
int fov_recompute;
int game_state;
int player_action;
Zone* zone;
TCODConsole* con;
Object* player;

inline
void find_empty_tile () {
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

inline 
void draw_all () {
  //Draw all objects on current zone
  for (list<Object*>::iterator o = zone->objects->begin ();
       o != zone->objects->end ();
       o++) {
    //Only show if it's visible to the player
    if (fov_map->isInFov ((*o)->x, (*o)->y)) {
      //Set the color and then draw the character for this object
      //con->setForegroundColor ((*o)->color);
      con->setCharForeground ((*o)->x, (*o)->y, (*o)->color);
      con->putChar ((*o)->x, (*o)->y, (*o)->c, TCOD_BKGND_NONE);
    }
  }
}

inline 
void clear_all () {
  //Clear all object characters from map
  for (list<Object*>::iterator o = zone->objects->begin ();
       o != zone->objects->end ();
       o++) {
    //Erase the character for this object
    con->putChar ((*o)->x, (*o)->y, ' ', TCOD_BKGND_NONE);
  }
}

void render_all () {
  //Draw all the stuff in the current zone
  //Recompute fov if we need to
  if (fov_recompute) {
    fov_recompute = false;
    fov_map->computeFov (player->x, player->y, TORCH_RADIUS, FOV_LIGHT_WALLS); 
  }
  //Draw the grid
  for (int y=0; y < GRID_HEIGHT; y++){
    for (int x=0; x < GRID_WIDTH; x++){
      int visible = fov_map->isInFov (x, y);
      int wall = zone->grid[x][y]->block_sight;
      if (!visible){
        if (zone->grid[x][y]->explored) {
          if (wall) {
            con->setCharBackground (x, y, color_dark_wall);
          }
          else {
            con->setCharBackground (x, y, color_dark_ground);
          }
        }
      }
      else {
        if (wall) {
          con->setCharBackground (x, y, color_light_wall);
        }
        else {
          con->setCharBackground (x, y, color_light_ground);
        }
        zone->grid[x][y]->explored = true;
      }        
    }
  }
  //Draw the objects
  draw_all();
  
  //Blit results to root console
  TCODConsole::blit (con, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, TCODConsole::root, 0, 0);
}

 
int handle_keys () {
  TCOD_key_t key = TCODConsole::waitForKeypress (true);
 
  if (key.vk == TCODK_ENTER && key.lalt) {
    TCODConsole::setFullscreen (!TCODConsole::isFullscreen());
    return (0);
  } 
  else if (key.vk == TCODK_ESCAPE) {
    return (-1);
  }

  if (game_state == GS_PLAYING) {
    //Movement keys
    if (TCODConsole::isKeyPressed (TCODK_UP)) {
      zone->move_object (player, 0, -1);
      fov_recompute = true;
    }
    else if (TCODConsole::isKeyPressed (TCODK_DOWN)) {
      zone->move_object(player, 0, 1);
      fov_recompute = true;
    }
    else if (TCODConsole::isKeyPressed(TCODK_LEFT)) {
      zone->move_object(player, -1, 0);
      fov_recompute = true;
    }
    else if (TCODConsole::isKeyPressed(TCODK_RIGHT)) {
      zone->move_object(player, 1, 0);
      fov_recompute = true;
    }
  }
  return (0);
}

int main (int argc, const char** argv) {
  TCODConsole::setCustomFont ("arial10x10.png", TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD);
  cerr << "+ Init root console" << endl;
  TCODConsole::initRoot (SCREEN_WIDTH, SCREEN_HEIGHT, "python/libtcod tutorial", false);
  TCODSystem::setFps (LIMIT_FPS);
  cerr << "+ Create offscreen console" << endl;
  con = new TCODConsole::TCODConsole (SCREEN_WIDTH, SCREEN_HEIGHT);

  cerr << "+ Create zone" << endl;
  zone = new Zone (GRID_WIDTH, GRID_HEIGHT);
  cerr << "+ Generate dungeon" << endl;
  make_grid (zone);

  cerr << "+ Create player" << endl;
  player = new Object (0, 0, '@', "player", TCODColor::white, true);
  cerr << "+ Place player" << endl;
  find_empty_tile ();
  zone->objects->push_back (player);    

  fov_map = new TCODMap::TCODMap (GRID_WIDTH, GRID_HEIGHT);
  for (int y=0; y < GRID_HEIGHT; y++)
    for (int x=0; x < GRID_WIDTH; x++) {
      fov_map->setProperties (x, y, !zone->grid[x][y]->blocked, !zone->grid[x][y]->block_sight);
    }
  fov_recompute = true;
  game_state = GS_PLAYING;
  player_action = PA_NONE;
  
  while (!TCODConsole::isWindowClosed ()) {
    cerr << "+ Rendering" << endl;
    render_all ();
    cerr << "+ Flushing" << endl;
    TCODConsole::flush ();
    cerr << "+ Clearing" << endl;
    clear_all();

    if (handle_keys () == -1) {    
      break;
    }
  }

  cerr << "+ Window closed" << endl;

  delete TCODConsole::root;
  delete con;
  delete fov_map;
  delete zone;
  cerr << "+ Memory deallocation completed" << endl;

  return(0);
}
