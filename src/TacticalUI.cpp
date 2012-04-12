#include "TacticalUI.hpp"


TacticalUI::TacticalUI (Zone* new_zone, list<AI*>* new_ais,
        Lifeform* new_player) {
    zone = new_zone;
    ais = new_ais;
    player = new_player;
    player_fov_map = zone->new_fov_map ();
    map_console_w = zone->width ();
    map_console_h = zone->height ();
    map_console = new TCODConsole::TCODConsole (map_console_w, map_console_h);
    hud_messages_console = new TCODConsole::TCODConsole (HUD_MESSAGES_W,
            HUD_MESSAGES_H);
    hud_target_console = new TCODConsole::TCODConsole (HUD_TARGET_W,
            HUD_TARGET_H);
    hud_status_console = new TCODConsole::TCODConsole (HUD_STATUS_W,
            HUD_STATUS_H);
    target = (Coord) {0,0};
    target_name = new char[1];
    strcpy (target_name, "");
    player_quit = false;
}

TacticalUI::~TacticalUI () {
    delete (player_fov_map);
    delete (map_console);
    delete (hud_messages_console);
    delete (hud_target_console);
    delete (hud_status_console);
    delete (target_name);
}

void TacticalUI::render_terrain () {
    //Draw the grid
    for (int y=0; y < zone->height (); y++){
        for (int x=0; x < zone->width (); x++){
            if (!player_fov_map->isInFov (x, y)){
                // Not visible
                if (zone->get_tile_explored ((Coord){x,y})) {
                    // Explored
                    float h, s, v;
                    (zone->get_tile_color ((Coord){x,y})).getHSV (&h, &s, &v);
                    TCODColor color = TCODColor (h, s * 2, v / 2);
                    map_console->setCharBackground (x, y, color);
                }
                else {
                    // Not explored
                    TCODColor color = TCODColor::black;
                    map_console->setCharBackground (x, y, color);
                }
            }
            else {
                // Visible
                TCODColor color = zone->get_tile_color ((Coord){x,y});
                map_console->setCharBackground (x, y, color);
                zone->set_tile_explored ((Coord){x,y}, true);
            }
        }
    }
}

void TacticalUI::render_objects () {
    //Draw all objects on current zone
    for (int i = 0; i < zone->width (); i++) {
        for (int j = 0; j < zone->height (); j++) {
            //Only show if it's visible to the player
            if (player_fov_map->isInFov (i,j)) {
                list<int>* temp = zone->objects_at ((Coord){i,j});
                if (!temp->empty ()) {
                    Object* o = Object::get_object_by_id (temp->front ());
                    //Set the color and then draw the character for this object
                    map_console->putChar (i, j, o->get_char (),
                            TCOD_BKGND_NONE);
                    map_console->setCharForeground (i, j,
                            o->get_color ());
                }
                delete (temp);
            }
        }
    }
}

void TacticalUI::clear_objects () {
    //Clear all object characters from map
    for (int i = 0; i < zone->width (); i++) {
        for (int j = 0; j < zone->height (); j++) {
            map_console->putChar (i, j, ' ', TCOD_BKGND_NONE);
        }
    }
}

void TacticalUI::bkgnd_rect (int x, int y, int w, int h, TCODColor color,
        TCODConsole* console) {
    assert (x >= 0 && y >= 0 &&
            x + w <= console->getWidth () && y + h <= console->getHeight ());
    for (int i = x; i < x + w; i++) {
        for (int j = y; j < y + h; j++) {
            console->setCharBackground (i, j, color);
        }
    }
}

void TacticalUI::bkgnd_hline (int x, int y, int l, TCODColor color,
        TCODConsole* console) {
    bkgnd_rect (x, y, l, 1, color, console);
}

void TacticalUI::bkgnd_vline (int x, int y, int l, TCODColor color,
        TCODConsole* console) {
    bkgnd_rect (x, y, 1, l, color, console);
}

void TacticalUI::draw_bkgnd_frame (int x, int y, int w, int h, TCODColor color,
        const char* str, TCODColor str_color, TCODConsole* console) {
    bkgnd_hline (x, y, w, color, console);
    bkgnd_hline (x, y + h - 1, w, color, console);
    bkgnd_vline (x, y, h, color, console);
    bkgnd_vline (x + w - 1, y, h, color, console);
    char* temp = new char[strlen (str) + 4];
    sprintf (temp, "%%c%s%%c", str);
    TCODConsole::setColorControl (TCOD_COLCTRL_1,
            TCODColor::black + TCODColor::darkerGrey, TCODColor::lightGrey);
    console->printEx ((x + w) / 2 - 1, y, TCOD_BKGND_SCREEN, TCOD_CENTER, temp,
            TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
    delete (temp);
}

void TacticalUI::render_hud () {
    // Set up color control code for HUD section labels (black on white)
    TCODConsole::setColorControl (TCOD_COLCTRL_1, TCODColor::darkerGrey,
            TCODColor::white);
    // Set up color control code for HUD section text (white on black)
    TCODConsole::setColorControl (TCOD_COLCTRL_2, TCODColor::white,
            TCODColor::black);

    // Draw Messages
    hud_messages_console->printFrame (0, 0, HUD_MESSAGES_W, HUD_MESSAGES_H,
            true, TCOD_BKGND_DEFAULT, "%cMESSAGES%c", TCOD_COLCTRL_1,
            TCOD_COLCTRL_STOP);

    // Draw Target
    hud_target_console->printFrame (0, 0, HUD_TARGET_W, HUD_TARGET_H, true,
            TCOD_BKGND_DEFAULT, "%cTARGET%c", TCOD_COLCTRL_1,
            TCOD_COLCTRL_STOP);
    char target_loc[12];
    char target_desc[5 + strlen (target_name)];
    sprintf (target_loc, "%%c(%02i,%02i)%%c", target.x, target.y);
    sprintf (target_desc, "%%c%s%%c", target_name);
    cout << target_loc << endl;
    cout << target_desc << endl;
    hud_target_console->printEx (HUD_TARGET_COORD_X, HUD_TARGET_COORD_Y,
            TCOD_BKGND_NONE, TCOD_LEFT, target_loc, TCOD_COLCTRL_2,
            TCOD_COLCTRL_STOP);
    hud_target_console->printEx (HUD_TARGET_DESC_X, HUD_TARGET_DESC_Y,
            TCOD_BKGND_NONE, TCOD_LEFT, target_desc, TCOD_COLCTRL_2,
            TCOD_COLCTRL_STOP);

    // Draw Status
    hud_status_console->printFrame (0, 0, HUD_STATUS_W, HUD_STATUS_H, true,
            TCOD_BKGND_DEFAULT, "%cSTATUS%c", TCOD_COLCTRL_1,
            TCOD_COLCTRL_STOP);
    char hp[7];
    sprintf(hp,"%%cHP: %3i%%c",player->get_hp ());
    hud_status_console->printEx (HUD_STATUS_HP_X, HUD_STATUS_HP_Y,
            TCOD_BKGND_NONE, TCOD_LEFT, hp, TCOD_COLCTRL_2,
            TCOD_COLCTRL_STOP);
}

void TacticalUI::blit_map_console () {
    TCODConsole::blit (map_console, 0, 0, map_console_w, map_console_h,
            TCODConsole::root, MAP_X, MAP_Y);
}

void TacticalUI::blit_hud_console () {
    TCODConsole::blit (hud_messages_console, 0, 0, HUD_MESSAGES_W,
            HUD_MESSAGES_H, TCODConsole::root, HUD_X + HUD_MESSAGES_X,
            HUD_Y + HUD_MESSAGES_Y);
    TCODConsole::blit (hud_target_console, 0, 0, HUD_TARGET_W, HUD_TARGET_H,
            TCODConsole::root, HUD_X + HUD_TARGET_X, HUD_Y + HUD_TARGET_Y);
    TCODConsole::blit (hud_status_console, 0, 0, HUD_STATUS_W, HUD_STATUS_H,
            TCODConsole::root, HUD_X + HUD_STATUS_X, HUD_Y + HUD_STATUS_Y);
}

void TacticalUI::move_target (Coord new_target) {
    if (zone->in_bounds (new_target)) {target = new_target;};
    list<int>* object_ids = zone->objects_at (target);
    delete (target_name);
    if (!object_ids->empty ()) {
        Object* o = Object::get_object_by_id (object_ids->front ());
        target_name = new char[strlen (o->get_name ())];
        strcpy (target_name, o->get_name ());
    }
    else {
        target_name = new char[1];
        strcpy (target_name, "");
    }
}

TargetData TacticalUI::targeter () {
    TCOD_key_t key = {TCODK_NONE,0};
    TCOD_mouse_t mouse;

    TCODConsole* targeter_console = new TCODConsole::TCODConsole (1,1);
    targeter_console->putChar (0, 0, 'X', TCOD_BKGND_NONE);
    targeter_console->setCharForeground (0, 0, targeter_color);

    target = zone->location_of (player->get_id ());

    while (1) {
        blit_map_console ();
        render_hud ();
        blit_hud_console ();
        TCODColor target_bk = map_console->getCharBackground (target.x,
                target.y);
        targeter_console->setCharBackground (0, 0, target_bk);
        TCODConsole::blit (targeter_console, 0, 0, 1, 1, TCODConsole::root,
                target.x, target.y);
        TCODConsole::flush ();

        TCODSystem::waitForEvent(
                (TCOD_event_t)(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE), &key,
                &mouse, false);
        // Fullscreen
        if (key.vk == TCODK_ENTER && (key.lalt || key.ralt)) {
            TCODConsole::setFullscreen (!TCODConsole::isFullscreen());
        }
        // Select target
        else if (key.vk == TCODK_ENTER && !(key.lalt || key.ralt)) {
            delete (targeter_console);
            return ((TargetData) {key, target});
        }
        // Cancel
        else if (key.vk == TCODK_ESCAPE) {
            delete (targeter_console);
            return ((TargetData) {key, ((Coord) {0,0})});
        }
        // Movement
        else if (key.vk == TCODK_CHAR && key.c == 'k') {
            //up
            move_target ((Coord) {target.x, target.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c =='j') {
            //down
            move_target ((Coord) {target.x, target.y + 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'h') {
            //left
            move_target ((Coord) {target.x - 1, target.y});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'l') {
            //right
            move_target ((Coord) {target.x + 1, target.y});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'y') {
            //up-left
            move_target ((Coord) {target.x - 1, target.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'u') {
            //up-right
            move_target ((Coord) {target.x + 1, target.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'b') {
            //down-left
            move_target ((Coord) {target.x  - 1, target.y + 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'n') {
            //down-right
            move_target ((Coord) {target.x + 1, target.y + 1});
        }
    }
}

AbilityInvocation* TacticalUI::handle_keys () {
    TCOD_key_t key = {TCODK_NONE,0};
    TCOD_mouse_t mouse;

    while (1) {
        //    TCOD_key_t key = TCODConsole::waitForKeypress (true);
        TCODSystem::waitForEvent (
                (TCOD_event_t) (TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE),
                &key, &mouse, false);

        if (key.vk == TCODK_ENTER && key.lalt) {
            TCODConsole::setFullscreen (!TCODConsole::isFullscreen());
            continue;
        }
        else if (key.vk == TCODK_ESCAPE) {
            player_quit = true;
            return (new NullInvocation (player->get_id (), zone));
        }
        //Movement keys
        else if (key.vk == TCODK_CHAR && key.c == 'k') {
            return (new StepInvocation (player->get_id (), zone, 8));
        }
        else if (key.vk == TCODK_CHAR && key.c =='j') {
            return (new StepInvocation (player->get_id (), zone, 2));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'h') {
            return (new StepInvocation (player->get_id (), zone, 4));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'l') {
            return (new StepInvocation (player->get_id (), zone, 6));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'y') {
            return (new StepInvocation (player->get_id (), zone, 7));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'u') {
            return (new StepInvocation (player->get_id (), zone, 9));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'b') {
            return (new StepInvocation (player->get_id (), zone, 1));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'n') {
            return (new StepInvocation (player->get_id (), zone, 3));
        }
        else if (key.vk == TCODK_CHAR && key.c == 'x') {
            // Call function to do targetting.
            targeter ();
            blit_map_console ();
            TCODConsole::flush ();
        }
        else {
            return (new NullInvocation (player->get_id (), zone));
        }
    }
}

int TacticalUI::display () {
    while (!TCODConsole::isWindowClosed ()) {
        Coord player_loc = zone->location_of (player->get_id ());
        player_fov_map->computeFov (player_loc.x, player_loc.y, TORCH_RADIUS,
                FOV_LIGHT_WALLS);

        render_terrain ();
        clear_objects ();
        render_objects ();
        blit_map_console ();

        render_hud ();
        blit_hud_console ();

        TCODConsole::flush ();

        AbilityInvocation* player_action = handle_keys ();
        if (player_quit) {
            break;
        }
        player_action->execute ();
        delete (player_action);

        for (list<AI*>::iterator ai = ais->begin ();
                ai != ais->end (); ai++) {
            AbilityInvocation* ai_action = (*ai)->take_turn ();
            ai_action->execute ();
            delete (ai_action);
        }
    }
    return (0);
}
