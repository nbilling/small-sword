#ifndef TACTICALUI_HPP
#define TACTICALUI_HPP

#include "list"
#include "iostream"
#include "libtcod.hpp"
#include "Types.hpp"
#include "DungeonGenerator.hpp"
#include "Zone.hpp"
#include "Object.hpp"
#include "AI.hpp"

#define color_dark_wall TCODColor::darkerGrey
#define color_light_wall TCODColor::grey
#define color_dark_ground TCODColor::darkerOrange
#define color_light_ground TCODColor::desaturatedOrange

#define HUD_HP_X 1
#define HUD_HP_Y 41

#define GS_PLAYING 1

typedef struct {
  TCOD_key_t key;
  Coord coord;
} TargetData;  

class TacticalUI {
private:
  Zone* zone;
  list<AI*>* ais;
  Object* player;
  TCODMap* player_fov_map;
  TCODConsole* map_console;
  int map_console_w;
  int map_console_h;
  TCODConsole* hud_console;
  int hud_console_w;
  int hud_console_h;

  bool fov_recompute;
  int game_state;

  void render_grid ();

  void render_objects ();

  void clear_objects ();

  void hud_write (int x, int y, const char* s);

  void render_hud ();

  TargetData targeter ();

  int handle_keys ();
 
public:
  TacticalUI (Zone* new_zone, list<AI*>* new_ais, Object* new_player, TCODMap* new_player_fov_map, 
              TCODConsole* new_map_console, int new_map_console_w, int new_map_console_h, 
              TCODConsole* new_hud_console, int new_hud_console_w, int new_hud_console_h);
  
  int display ();
};

#endif
