#include "DungeonGenerator.cpp"

#define FOV_ALGO 0  
#define FOV_LIGHT_WALLS true
#define TORCH_RADIUS 10

#define color_dark_wall TCODColor::darkGrey
#define color_light_wall TCODColor::lightGrey
#define color_dark_ground TCODColor::darkOrange
#define color_light_ground TCODColor::desaturatedOrange

#define GS_PLAYING 1
#define PA_NONE 0

class TacticalUI {
public:
  Zone* zone;
  Object* player;
  TCODMap* fov_map;
  TCODConsole* console;
  int screen_w;
  int screen_h;

  bool fov_recompute;
  int game_state;
  int player_action;

  TacticalUI (Zone* new_zone, Object* new_player, TCODMap* new_fov_map, 
                     TCODConsole* new_console, int new_screen_w, int new_screen_h) {
    zone = new_zone;
    player = new_player;
    fov_map = new_fov_map;
    console = new_console;
    screen_w = new_screen_w;
    screen_h = new_screen_h;
  }
  
  void render_grid () {
    //Draw the grid 
    for (int y=0; y < zone->grid_h; y++){
      for (int x=0; x < zone->grid_w; x++){
        int visible = fov_map->isInFov (x, y);
        int wall = zone->grid[x][y]->block_sight;
        if (!visible){
          if (zone->grid[x][y]->explored) {
            if (wall) {
              console->setCharBackground (x, y, color_dark_wall);
            }
            else {
              console->setCharBackground (x, y, color_dark_ground);
            }
          }
        }
        else {
          if (wall) {
            console->setCharBackground (x, y, color_light_wall);
          }
          else {
            console->setCharBackground (x, y, color_light_ground);
          }
          zone->grid[x][y]->explored = true;
        }        
      }
    }
  }

  void render_objects () {
    //Draw all objects on current zone
    for (list<Object*>::iterator o = zone->objects->begin ();
         o != zone->objects->end ();
         o++) {
      //Only show if it's visible to the player
      if (fov_map->isInFov ((*o)->x, (*o)->y)) {
        //Set the color and then draw the character for this object
        console->setCharForeground ((*o)->x, (*o)->y, (*o)->color);
        console->putChar ((*o)->x, (*o)->y, (*o)->c, TCOD_BKGND_NONE);
      }
    }
  }

  void clear_objects () {
    //Clear all object characters from map
    for (list<Object*>::iterator o = zone->objects->begin ();
         o != zone->objects->end ();
         o++) {
      //Erase the character for this object
      console->putChar ((*o)->x, (*o)->y, ' ', TCOD_BKGND_NONE);
    }
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
  
  int display () {
    fov_recompute = true;
    game_state = GS_PLAYING;
    player_action = PA_NONE;
    
    while (!TCODConsole::isWindowClosed ()) {
      if (fov_recompute) {
        fov_recompute = false;
        fov_map->computeFov (player->x, player->y, TORCH_RADIUS, FOV_LIGHT_WALLS); 
      }
      render_grid ();
      render_objects ();
      TCODConsole::blit (console, 0, 0, screen_w, screen_h, TCODConsole::root, 0, 0);
      TCODConsole::flush ();
      clear_objects ();
      
      if (handle_keys () == -1) {    
        break;
      }
    }
    return (0);
  }


};
