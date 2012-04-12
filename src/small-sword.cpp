#include "iostream"
#include "list"
#include "libtcod.hpp"
#include "AI.hpp"
#include "TacticalUI.hpp"
#include "Zone.hpp"
#include "DungeonGenerator.hpp"
using namespace std;

//Definitions
#define ROOT_CONSOLE_WIDTH 100
#define ROOT_CONSOLE_HEIGHT 50

#define MAP_CONSOLE_WIDTH 80
#define MAP_CONSOLE_HEIGHT 50

#define GRID_WIDTH 80
#define GRID_HEIGHT 45

#define LIMIT_FPS 60


void find_empty_tile (Zone* zone, Object* player) {
    //Put player in first unblocked tile found
    for (int x=0; x < GRID_WIDTH; x++) {
        for (int y=0; y< GRID_HEIGHT; y++) {
            if (!zone->is_blocked ((Coord){x, y})) {
                zone->place_object(player->get_id (), (Coord){x, y});
                return;
            }
        }
    }
}

int main (int argc, const char** argv) {
    TCODConsole::setCustomFont ("data/arial10x10.png",
            TCOD_FONT_TYPE_GREYSCALE | TCOD_FONT_LAYOUT_TCOD);
    cerr << "+ Init root console" << endl;
    TCODConsole::initRoot (ROOT_CONSOLE_WIDTH, ROOT_CONSOLE_HEIGHT,
            "small-sword", false);
    TCODSystem::setFps (LIMIT_FPS);

    cerr << "+ Create zone" << endl;
    Zone* zone = new Zone (GRID_WIDTH, GRID_HEIGHT);

    cerr << "+ Create AI list" << endl;
    list<AI*>* ais = new list<AI*> ();

    cerr << "+ Generate dungeon" << endl;
    make_grid (zone, ais);

    cerr << "+ Create player" << endl;
    Lifeform* player = new Lifeform ('@', "player", TCODColor::white, true, 20);
    Weapon* sword = new Weapon (')', "sword", TCODColor::darkerGrey, false,
            true, 2, 1);
    player->equip_right_hand (sword->get_id ());

    cerr << "+ Place player" << endl;
    find_empty_tile (zone, player);

    cerr << "+ Create TacticalUI" << endl;
    TacticalUI* tactical = new TacticalUI (zone, ais, player);

    cerr << "+ Initialisation complete" << endl;

    tactical->display ();

    cerr << "+ Window closed" << endl;

    delete tactical;
    delete TCODConsole::root;
    delete zone;
    Object::delete_registered_objects ();
    cerr << "+ Memory deallocation completed" << endl;

    return(0);
}
