#include "TacticalUI.hpp"


TacticalUI::TacticalUI (Zone* new_zone, list<AI*>* new_ais,
        Lifeform* new_player) {
    zone = new_zone;

    ais = new_ais;

    player = new_player;
    player_fov_map = zone->new_fov_map ();
    player_quit = false;

    map_console = new TCODConsole::TCODConsole
        (zone->width (), zone->height ());
    map_console_w = zone->width ();
    map_console_h = zone->height ();

    hud_frame_messages_console = new TCODConsole::TCODConsole
        (HUD_FRAME_MESSAGES_W, HUD_FRAME_MESSAGES_H);
    hud_messages_console = new TCODConsole::TCODConsole
        (HUD_MESSAGES_W, HUD_MESSAGES_H);
    messages_terminal = new Terminal (HUD_MESSAGES_W);

    hud_frame_target_console = new TCODConsole::TCODConsole
        (HUD_FRAME_TARGET_W, HUD_FRAME_TARGET_H);
    hud_target_console = new TCODConsole::TCODConsole
        (HUD_TARGET_W, HUD_TARGET_H);
    target_coord = (Coord) {0,0};
    target_id = 0;

    hud_frame_status_console = new TCODConsole::TCODConsole
        (HUD_FRAME_STATUS_W, HUD_FRAME_STATUS_H);
    hud_status_console = new TCODConsole::TCODConsole
        (HUD_STATUS_W, HUD_STATUS_H);
}

TacticalUI::~TacticalUI () {
    delete (player_fov_map);
    delete (map_console);
    delete (hud_frame_messages_console);
    delete (hud_frame_target_console);
    delete (hud_frame_status_console);
    delete (hud_messages_console);
    delete (hud_target_console);
    delete (hud_status_console);
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
                list<ObjId>* temp = zone->objects_at ((Coord){i,j});
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

void TacticalUI::render_map () {
    render_terrain ();
    clear_objects ();
    render_objects ();
}

void TacticalUI::render_hud_messages () {
    // Set up color control code for HUD section labels (black on white)
    TCODConsole::setColorControl (TCOD_COLCTRL_1, TCODColor::darkerGrey,
            TCODColor::white);
    // clear the damn hud_messages_console here!!!
    hud_frame_messages_console->printFrame (0, 0, HUD_FRAME_MESSAGES_W,
            HUD_FRAME_MESSAGES_H, true, TCOD_BKGND_DEFAULT, "%cMESSAGES%c",
            TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
    for (int x=0; x < HUD_MESSAGES_W; x++)
        for (int y=0; y < HUD_MESSAGES_H; y++)
            hud_messages_console->putChar (x, y, ' ', TCOD_BKGND_NONE);
    list<string>* lines = messages_terminal->get_lines (HUD_MESSAGES_H);
    list<string>::reverse_iterator it = lines->rbegin ();
    for (int i = HUD_MESSAGES_H - 1; i >= 0 && it != lines->rend (); i--) {
        const char* temp = ("%c" + *it + "%c").c_str ();
        hud_messages_console->printEx (0, i, TCOD_BKGND_NONE, TCOD_LEFT, temp,
                TCOD_COLCTRL_2, TCOD_COLCTRL_STOP);
        it++;
    }
}

void TacticalUI::render_hud_target () {
    // Set up color control code for HUD section labels (black on white)
    TCODConsole::setColorControl (TCOD_COLCTRL_1, TCODColor::darkerGrey,
            TCODColor::white);
    hud_frame_target_console->printFrame (0, 0, HUD_FRAME_TARGET_W,
            HUD_FRAME_TARGET_H, true, TCOD_BKGND_DEFAULT, "%cTARGET%c",
            TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);

    // Set up color control code for HUD section text (white on black)
    TCODConsole::setColorControl (TCOD_COLCTRL_2, TCODColor::white,
            TCODColor::black);
    char target_loc[12];
    sprintf (target_loc, "%%c(%02i,%02i)%%c", target_coord.x, target_coord.y);
    hud_target_console->printEx (HUD_TARGET_COORD_X, HUD_TARGET_COORD_Y,
            TCOD_BKGND_NONE, TCOD_LEFT, target_loc, TCOD_COLCTRL_2,
            TCOD_COLCTRL_STOP);
    if (target_id) {
        Object* target = Object::get_object_by_id (target_id);
        string target_name = target->get_name ();
        const char* target_desc = ("%c" + target_name + "%c").c_str ();
        hud_target_console->printEx (HUD_TARGET_DESC_X, HUD_TARGET_DESC_Y,
                TCOD_BKGND_NONE, TCOD_LEFT, target_desc, TCOD_COLCTRL_2,
                TCOD_COLCTRL_STOP);
        if (target->type () == LifeformType) {
            ObjId target_R_object_id =
                ((Lifeform*) target)->get_equipped_right_hand ();
            if (target_R_object_id) {
                string target_R_object_name =
                    (Object::get_object_by_id
                     (target_R_object_id))->get_name ();
                const char* target_R =
                    ("%c" + target_R_object_name + "%c").c_str ();
                hud_target_console->printEx (HUD_TARGET_R_X, HUD_TARGET_R_Y,
                        TCOD_BKGND_NONE, TCOD_LEFT, target_R, TCOD_COLCTRL_2,
                        TCOD_COLCTRL_STOP);
            }
        }
    }
}

void TacticalUI::render_hud_status () {
    // Set up color control code for HUD section labels (black on white)
    TCODConsole::setColorControl (TCOD_COLCTRL_1, TCODColor::darkerGrey,
            TCODColor::white);
    hud_frame_status_console->printFrame (0, 0, HUD_FRAME_STATUS_W,
            HUD_FRAME_STATUS_H, true, TCOD_BKGND_DEFAULT, "%cSTATUS%c",
            TCOD_COLCTRL_1, TCOD_COLCTRL_STOP);
    // Set up color control code for HUD section text (white on black)
    TCODConsole::setColorControl (TCOD_COLCTRL_2, TCODColor::white,
            TCODColor::black);
    char hp[7];
    sprintf(hp,"%%cHP: %3i%%c",player->get_hp ());
    hud_status_console->printEx (HUD_STATUS_HP_X, HUD_STATUS_HP_Y,
            TCOD_BKGND_NONE, TCOD_LEFT, hp, TCOD_COLCTRL_2,
            TCOD_COLCTRL_STOP);
    ObjId R_object_id = player->get_equipped_right_hand ();
    if (R_object_id) {
        string R_object_name =
            (Object::get_object_by_id (R_object_id))->get_name ();
        const char* r = ("%cR: " + R_object_name + "%c").c_str ();
        hud_status_console->printEx (HUD_STATUS_R_X, HUD_STATUS_R_Y,
                TCOD_BKGND_NONE, TCOD_LEFT, r, TCOD_COLCTRL_2,
                TCOD_COLCTRL_STOP);
    }
}

void TacticalUI::render_hud () {
    render_hud_messages ();
    render_hud_target ();
    render_hud_status ();
}

void TacticalUI::blit_map () {
    TCODConsole::blit (map_console, 0, 0, map_console_w, map_console_h,
            TCODConsole::root, MAP_X, MAP_Y);
}

void TacticalUI::blit_hud_messages () {
    TCODConsole::blit (hud_frame_messages_console, 0, 0, HUD_FRAME_MESSAGES_W,
            HUD_FRAME_MESSAGES_H, TCODConsole::root,
            HUD_X + HUD_FRAME_MESSAGES_X,
            HUD_Y + HUD_FRAME_MESSAGES_Y);
    TCODConsole::blit (hud_messages_console, 0, 0, HUD_MESSAGES_W,
            HUD_MESSAGES_H, TCODConsole::root, HUD_X + HUD_MESSAGES_X,
            HUD_Y + HUD_MESSAGES_Y);
}

void TacticalUI::blit_hud_target () {
    TCODConsole::blit (hud_frame_target_console, 0, 0, HUD_FRAME_TARGET_W,
            HUD_FRAME_TARGET_H, TCODConsole::root,
            HUD_X + HUD_FRAME_TARGET_X, HUD_Y + HUD_FRAME_TARGET_Y);
    TCODConsole::blit (hud_target_console, 0, 0, HUD_TARGET_W, HUD_TARGET_H,
            TCODConsole::root, HUD_X + HUD_TARGET_X, HUD_Y + HUD_TARGET_Y);
}

void TacticalUI::blit_hud_status () {
    TCODConsole::blit (hud_frame_status_console, 0, 0, HUD_FRAME_STATUS_W,
            HUD_FRAME_STATUS_H, TCODConsole::root, HUD_X + HUD_FRAME_STATUS_X,
            HUD_Y + HUD_FRAME_STATUS_Y);
    TCODConsole::blit (hud_status_console, 0, 0, HUD_STATUS_W, HUD_STATUS_H,
            TCODConsole::root, HUD_X + HUD_STATUS_X, HUD_Y + HUD_STATUS_Y);
}

void TacticalUI::blit_hud () {
    blit_hud_messages ();
    blit_hud_target ();
    blit_hud_status ();
}

void TacticalUI::move_target (Coord new_target) {
    if (zone->in_bounds (new_target))
        target_coord = new_target;
    list<ObjId>* object_ids = zone->objects_at (target_coord);
    if (!object_ids->empty ())
        target_id = object_ids->front ();
    else
        target_id = 0;
}

ObjId TacticalUI::object_targeter () {
    TCOD_key_t key = {TCODK_NONE,0};
    TCOD_mouse_t mouse;

    TCODConsole* targeter_console = new TCODConsole::TCODConsole (1,1);
    targeter_console->putChar (0, 0, 'X', TCOD_BKGND_NONE);
    targeter_console->setCharForeground (0, 0, targeter_color);

    target_coord = zone->location_of (player->get_id ());

    while (1) {
        blit_map ();
        render_hud ();
        blit_hud ();
        TCODColor target_bk = map_console->getCharBackground (target_coord.x,
                target_coord.y);
        targeter_console->setCharBackground (0, 0, target_bk);
        TCODConsole::blit (targeter_console, 0, 0, 1, 1, TCODConsole::root,
                target_coord.x, target_coord.y);
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
            return (target_id);
        }
        // Cancel
        else if (key.vk == TCODK_ESCAPE) {
            delete (targeter_console);
            return (ObjId (0));
        }
        // Movement
        else if (key.vk == TCODK_CHAR && key.c == 'k') {
            //up
            move_target ((Coord) {target_coord.x, target_coord.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c =='j') {
            //down
            move_target ((Coord) {target_coord.x, target_coord.y + 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'h') {
            //left
            move_target ((Coord) {target_coord.x - 1, target_coord.y});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'l') {
            //right
            move_target ((Coord) {target_coord.x + 1, target_coord.y});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'y') {
            //up-left
            move_target ((Coord) {target_coord.x - 1, target_coord.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'u') {
            //up-right
            move_target ((Coord) {target_coord.x + 1, target_coord.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'b') {
            //down-left
            move_target ((Coord) {target_coord.x  - 1, target_coord.y + 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'n') {
            //down-right
            move_target ((Coord) {target_coord.x + 1, target_coord.y + 1});
        }
    }
}

Coord TacticalUI::tile_targeter () {
    TCOD_key_t key = {TCODK_NONE,0};
    TCOD_mouse_t mouse;

    TCODConsole* targeter_console = new TCODConsole::TCODConsole (1,1);
    targeter_console->putChar (0, 0, 'X', TCOD_BKGND_NONE);
    targeter_console->setCharForeground (0, 0, targeter_color);

    target_coord = zone->location_of (player->get_id ());

    while (1) {
        blit_map ();
        render_hud ();
        blit_hud ();
        TCODColor target_bk = map_console->getCharBackground (target_coord.x,
                target_coord.y);
        targeter_console->setCharBackground (0, 0, target_bk);
        TCODConsole::blit (targeter_console, 0, 0, 1, 1, TCODConsole::root,
                target_coord.x, target_coord.y);
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
            return (target_coord);
        }
        // Cancel
        else if (key.vk == TCODK_ESCAPE) {
            delete (targeter_console);
            return ((Coord) {-1,-1});
        }
        // Movement
        else if (key.vk == TCODK_CHAR && key.c == 'k') {
            //up
            move_target ((Coord) {target_coord.x, target_coord.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c =='j') {
            //down
            move_target ((Coord) {target_coord.x, target_coord.y + 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'h') {
            //left
            move_target ((Coord) {target_coord.x - 1, target_coord.y});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'l') {
            //right
            move_target ((Coord) {target_coord.x + 1, target_coord.y});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'y') {
            //up-left
            move_target ((Coord) {target_coord.x - 1, target_coord.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'u') {
            //up-right
            move_target ((Coord) {target_coord.x + 1, target_coord.y - 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'b') {
            //down-left
            move_target ((Coord) {target_coord.x  - 1, target_coord.y + 1});
        }
        else if (key.vk == TCODK_CHAR && key.c == 'n') {
            //down-right
            move_target ((Coord) {target_coord.x + 1, target_coord.y + 1});
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
            tile_targeter ();
            blit_map ();
            TCODConsole::flush ();
        }
        else if (key.vk == TCODK_CHAR && key.c == 'a') {
            ObjId objid = object_targeter ();
            blit_map ();
            TCODConsole::flush ();
            if (objid) {
                if ((Object::get_object_by_id (objid))->type () == LifeformType)
                    return (new AttackInvocation (player->get_id (), zone,
                                objid));
            }
        }
        else {
            return (new NullInvocation (player->get_id (), zone));
        }
    }
}

int TacticalUI::display () {
    messages_terminal->append ("Welcome to small-sword tech-demo");
    messages_terminal->append ("Enjoy your stay");
    while (!TCODConsole::isWindowClosed ()) {
        Coord player_loc = zone->location_of (player->get_id ());
        player_fov_map->computeFov (player_loc.x, player_loc.y, TORCH_RADIUS,
                FOV_LIGHT_WALLS);

        render_map ();
        blit_map ();

        render_hud ();
        blit_hud ();

        TCODConsole::flush ();

        AbilityInvocation* player_action = handle_keys ();
        if (player_quit) {
            break;
        }
        if (player->get_hp () <= 0) {
            break;
        }
        player_action->execute ();
        cout << player_action->message ();
        messages_terminal->append (player_action->message ());
        delete (player_action);

        for (list<AI*>::iterator ai = ais->begin ();
                ai != ais->end (); ai++) {
            if ((*ai)->is_dead ()) {
                delete (*ai);
                ais->erase (ai);
            }
            else {
                AbilityInvocation* ai_action = (*ai)->take_turn ();
                ai_action->execute ();
                delete (ai_action);
            }
        }
    }
    return (0);
}
