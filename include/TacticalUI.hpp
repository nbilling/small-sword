#ifndef TACTICALUI_HPP
#define TACTICALUI_HPP

#include "list"
#include "string"
#include "iostream"
#include "libtcod.hpp"
#include "Types.hpp"
#include "DungeonGenerator.hpp"
#include "Zone.hpp"
#include "Object.hpp"
#include "AI.hpp"
using namespace std;

// Map dimensions
#define MAP_X 0
#define MAP_Y 0

// HUD and HUD section dimensions
// Note: HUD_X,HUD_Y is an offset in the root console whereas
//       HUD_<section>_X/HUD_<section>_Y is an offset in the hud console.
#define HUD_X 80
#define HUD_Y 0
#define HUD_W 20
#define HUD_H 45

#define HUD_MESSAGES_X 0
#define HUD_MESSAGES_Y 1      // Messages, Target, and Status heights add up to 44
#define HUD_MESSAGES_W HUD_W  // because Messages starts at (0,1).
#define HUD_MESSAGES_H 30

#define HUD_TARGET_X 0
#define HUD_TARGET_Y 31
#define HUD_TARGET_W HUD_W
#define HUD_TARGET_H 7

#define HUD_STATUS_X 0
#define HUD_STATUS_Y 38
#define HUD_STATUS_W HUD_W
#define HUD_STATUS_H 7

// Dimensions of fields in Target section.
#define HUD_TARGET_COORD_X HUD_TARGET_X + 1
#define HUD_TARGET_COORD_Y HUD_TARGET_Y + 1

#define HUD_TARGET_DESC_X HUD_TARGET_X + 1
#define HUD_TARGET_DESC_Y HUD_TARGET_Y + 2

// Dimensions of fields in Status section.
#define HUD_STATUS_HP_X HUD_STATUS_X + 1
#define HUD_STATUS_HP_Y HUD_STATUS_Y + 1

// HUD color scheme.
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
        Coord target;
        char* target_desc;

        bool player_quit;

        void render_terrain ();

        void render_objects ();

        void clear_objects ();

        void hud_write (int x, int y, const char* s);

        void bkgnd_rect (int x, int y, int w, int h, TCODColor color, TCODConsole* console);

        void bkgnd_hline (int x, int y, int l, TCODColor color, TCODConsole* console);

        void bkgnd_vline (int x, int y, int l, TCODColor color, TCODConsole* console);

        void draw_bkgnd_frame (int x, int y, int w, int h, TCODColor color,
                const char* str, TCODColor str_color, TCODConsole* console);

        void render_hud ();

        void blit_map_console ();

        void blit_hud_console ();

        void move_target (Coord new_target);

        TargetData targeter ();

        AbilityInvocation* handle_keys ();

    public:
        TacticalUI (Zone* new_zone, list<AI*>* new_ais, Lifeform* new_player);

        ~TacticalUI ();

        int display ();
};

#endif
