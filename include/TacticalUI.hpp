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
using namespace std;

#define HUD_HP_X 1
#define HUD_HP_Y 39

#define HUD_TARGET_X 1
#define HUD_TARGET_Y 34

#define targeter_color TCODColor::blue

typedef struct {
  TCOD_key_t key;
  Coord coord;
} TargetData;

class TacticalUI {
private:
  Zone* zone;
  list<AI*>* ais;
  Lifeform* player;
  TCODMap* player_fov_map;
  TCODConsole* map_console;
  int map_console_w;
  int map_console_h;
  TCODConsole* hud_console;
  int hud_console_w;
  int hud_console_h;
  Coord target;

  bool player_quit;

  void render_terrain ();

  void render_objects ();

  void clear_objects ();

  void hud_write (int x, int y, const char* s);

  void render_hud ();

  void blit_map_console ();

  void blit_hud_console ();

  void move_target (Coord new_target);

  TargetData targeter ();

  AbilityInvocation* handle_keys ();

public:
  TacticalUI (Zone* new_zone, list<AI*>* new_ais,
          Lifeform* new_player,
          int new_map_console_w, int new_map_console_h,
          int new_hud_console_w, int new_hud_console_h);

  ~TacticalUI ();

  int display ();
};

#endif
