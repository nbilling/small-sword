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
#define HUD_X 80
#define HUD_Y 0
#define HUD_W 20
#define HUD_H 45

#define HUD_FRAME_MESSAGES_X 0
#define HUD_FRAME_MESSAGES_Y 0
#define HUD_FRAME_MESSAGES_W HUD_W
#define HUD_FRAME_MESSAGES_H 30

#define HUD_FRAME_TARGET_X 0
#define HUD_FRAME_TARGET_Y 30
#define HUD_FRAME_TARGET_W HUD_W
#define HUD_FRAME_TARGET_H 7

#define HUD_FRAME_STATUS_X 0
#define HUD_FRAME_STATUS_Y 37
#define HUD_FRAME_STATUS_W HUD_W
#define HUD_FRAME_STATUS_H 7

#define HUD_MESSAGES_X HUD_FRAME_MESSAGES_X + 1
#define HUD_MESSAGES_Y HUD_FRAME_MESSAGES_Y + 1
#define HUD_MESSAGES_W HUD_FRAME_MESSAGES_W - 2
#define HUD_MESSAGES_H HUD_FRAME_MESSAGES_H - 2

#define HUD_TARGET_X HUD_FRAME_TARGET_X + 1
#define HUD_TARGET_Y HUD_FRAME_TARGET_Y + 1
#define HUD_TARGET_W HUD_FRAME_TARGET_W - 2
#define HUD_TARGET_H HUD_FRAME_TARGET_H - 2

#define HUD_STATUS_X HUD_FRAME_STATUS_X + 1
#define HUD_STATUS_Y HUD_FRAME_STATUS_Y + 1
#define HUD_STATUS_W HUD_FRAME_STATUS_W - 2
#define HUD_STATUS_H HUD_FRAME_STATUS_H - 2

// Dimensions of fields in Target section.
#define HUD_TARGET_COORD_X 0
#define HUD_TARGET_COORD_Y 0

#define HUD_TARGET_DESC_X 0
#define HUD_TARGET_DESC_Y 1

#define HUD_TARGET_R_X 0
#define HUD_TARGET_R_Y 2

// Dimensions of fields in Status section.
#define HUD_STATUS_HP_X 0
#define HUD_STATUS_HP_Y 0

#define HUD_STATUS_R_X 0
#define HUD_STATUS_R_Y 1

// HUD color scheme.
#define targeter_color TCODColor::blue

class TacticalUI {
    private:
        Zone* zone;
        list<AI*>* ais;
        Lifeform* player;
        TCODMap* player_fov_map;
        TCODConsole* map_console;
        int map_console_w;
        int map_console_h;
        TCODConsole* hud_frame_messages_console;
        TCODConsole* hud_frame_target_console;
        TCODConsole* hud_frame_status_console;
        TCODConsole* hud_messages_console;
        TCODConsole* hud_target_console;
        TCODConsole* hud_status_console;
        Coord target_coord;
        ObjId target_id;
        bool player_quit;

        void render_terrain ();

        void render_objects ();

        void clear_objects ();

        void render_map ();

        void render_hud_messages ();

        void render_hud_target ();

        void render_hud_status ();

        void render_hud ();

        void blit_map ();

        void blit_hud_messages ();

        void blit_hud_target ();

        void blit_hud_status ();

        void blit_hud ();

        void move_target (Coord new_target);

        ObjId object_targeter ();

        Coord tile_targeter ();

        AbilityInvocation* handle_keys ();

    public:
        TacticalUI (Zone* new_zone, list<AI*>* new_ais, Lifeform* new_player);

        ~TacticalUI ();

        int display ();
};

#endif
