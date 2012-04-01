#include "TacticalUI.hpp"


TacticalUI::TacticalUI (Zone* new_zone, list<AI*>* new_ais, Object* new_player, 
                        TCODMap* new_player_fov_map, TCODConsole* new_map_console, 
                        int new_map_console_w, int new_map_console_h, 
                        TCODConsole* new_hud_console, int new_hud_console_w, 
                        int new_hud_console_h) {
  zone = new_zone;
  ais = new_ais;
  player = new_player;
  player_fov_map = new_player_fov_map;
  map_console = new_map_console;
  map_console_w = new_map_console_w;
  map_console_h = new_map_console_h;
  hud_console = new_hud_console;
  hud_console_w = new_hud_console_w;
  hud_console_h = new_hud_console_h;
  player_quit = false;
}
  
void TacticalUI::render_grid () {
  //Draw the grid 
  for (int y=0; y < zone->grid_h; y++){
    for (int x=0; x < zone->grid_w; x++){
      if (!player_fov_map->isInFov (x, y)){
        // Not visible
        if (zone->grid[x][y]->explored) {
          // Explored
          float h, s, v;
          zone->grid[x][y]->color.getHSV (&h, &s, &v);
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
        TCODColor color = zone->grid[x][y]->color;
        map_console->setCharBackground (x, y, color);
        zone->grid[x][y]->explored = true;
      }
    }
  }
}

void TacticalUI::render_objects () {
  //Draw all objects on current zone
  for (int i = 0; i < zone->grid_w; i++) {
    for (int j = 0; j < zone->grid_h; j++) {
      //Only show if it's visible to the player        
      if (player_fov_map->isInFov (i,j)) {
        list<int>* temp = zone->objects_at ((Coord){i,j});
        if (!temp->empty ()) {
          Object* o = Object::get_object_by_id (temp->front ());
          //Set the color and then draw the character for this object
          map_console->putChar (i, j, o->c, TCOD_BKGND_NONE);
          map_console->setCharForeground (i, j, o->color);
        }
        delete (temp);
      }
    }
  }
}

void TacticalUI::clear_objects () {
  //Clear all object characters from map
  for (int i = 0; i < zone->grid_w; i++) {
    for (int j = 0; j < zone->grid_h; j++) {
      map_console->putChar (i, j, ' ', TCOD_BKGND_NONE);
    }
  }
}

void TacticalUI::hud_write (int x, int y, const char* s) {
  //Write string s (horizontally) to the hud at (x,y)
  //Will write to edge of console, dropping rest of string, if s is too long to fit 
  //from x to the end of the HUD console
  for (int i=0; (s[i] != '\0') && (i + x < hud_console_w); i++){
    hud_console->setCharForeground (i + x, y, TCODColor::white);
    hud_console->putChar (i + x, y, s[i], TCOD_BKGND_NONE);
  }
}

void TacticalUI::render_hud () {
  //Draw HUD
  char hp[7];
  sprintf(hp,"HP: %3i",player->csheet->hp);
  hud_write (HUD_HP_X, HUD_HP_Y, hp);
}

TargetData TacticalUI::targeter () {
  TCOD_key_t key = {TCODK_NONE,0};
  TCOD_mouse_t mouse;

  TCODConsole* targeter_console = new TCODConsole::TCODConsole (1,1);
  targeter_console->putChar (0, 0, 'X', TCOD_BKGND_NONE);
  targeter_console->setCharForeground (0, 0, targeter_color);

  Coord coord = zone->location_of (player->id);

  while (1) {
    TCODConsole::blit (map_console, 0, 0, map_console_w, map_console_h, 
                       TCODConsole::root, 0, 0);
    TCODColor coord_bk = map_console->getCharBackground (coord.x, coord.y);
    targeter_console->setCharBackground (0, 0, coord_bk);
    TCODConsole::blit (targeter_console, 0, 0, 1, 1, 
                       TCODConsole::root, coord.x, coord.y);
    TCODConsole::flush ();

  TCODSystem::waitForEvent((TCOD_event_t)(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE),&key,&mouse, false);
    // Fullscreen
    if (key.vk == TCODK_ENTER && (key.lalt || key.ralt)) {
      TCODConsole::setFullscreen (!TCODConsole::isFullscreen());
    }
    // Select target
    else if (key.vk == TCODK_ENTER && !(key.lalt || key.ralt)) {
      delete (targeter_console);
      return ((TargetData) {key, coord});
    }
    // Cancel
    else if (key.vk == TCODK_ESCAPE) {
      delete (targeter_console);
      return ((TargetData) {key, ((Coord) {0,0})});
    }
    // Movement
    else if (key.vk == TCODK_CHAR && key.c == 'k') {
      //up
      coord.y--;
    }
    else if (key.vk == TCODK_CHAR && key.c =='j') {
      //down
      coord.y++;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'h') {
      //left
      coord.x--;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'l') {
      //right
      coord.x++;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'y') {
      //up-left
      coord.x--;
      coord.y--;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'u') {
      //up-right
      coord.x++;
      coord.y--;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'b') {
      //down-left
      coord.x--;
      coord.y++;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'n') {
      //down-right
      coord.x++;
      coord.y++;
    }
  }
}

AbilityInvocation* TacticalUI::handle_keys () {
  TCOD_key_t key = {TCODK_NONE,0};
  TCOD_mouse_t mouse;

  while (1) {
    //    TCOD_key_t key = TCODConsole::waitForKeypress (true);
    TCODSystem::waitForEvent ((TCOD_event_t)(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE),
                              &key, &mouse, false);

    if (key.vk == TCODK_ENTER && key.lalt) {
      TCODConsole::setFullscreen (!TCODConsole::isFullscreen());
      continue;
    } 
    else if (key.vk == TCODK_ESCAPE) {
      player_quit = true;
      return (new NullInvocation (player->id, zone));
    }
      //Movement keys
    else if (key.vk == TCODK_CHAR && key.c == 'k') {
      return (new StepInvocation (player->id, zone, 8));
    }
    else if (key.vk == TCODK_CHAR && key.c =='j') {
      return (new StepInvocation (player->id, zone, 2));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'h') {
      return (new StepInvocation (player->id, zone, 4));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'l') {
      return (new StepInvocation (player->id, zone, 6));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'y') {
      return (new StepInvocation (player->id, zone, 7));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'u') {
      return (new StepInvocation (player->id, zone, 9));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'b') {
      return (new StepInvocation (player->id, zone, 1));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'n') {
      return (new StepInvocation (player->id, zone, 3));
    }
    else if (key.vk == TCODK_CHAR && key.c == 'x') {
      // Call function to do targetting.
      targeter ();
      TCODConsole::blit (map_console, 0, 0, map_console_w, map_console_h, 
                         TCODConsole::root, 0, 0);
      TCODConsole::flush ();
    }
    else {
      return (new NullInvocation (player->id, zone));
    }
  }
}
  
int TacticalUI::display () {
  while (!TCODConsole::isWindowClosed ()) {
    Coord player_loc = zone->location_of (player->id);
    player_fov_map->computeFov (player_loc.x, player_loc.y, TORCH_RADIUS, 
                                FOV_LIGHT_WALLS); 

    render_grid ();
    clear_objects ();
    render_objects ();

    TCODConsole::blit (map_console, 0, 0, map_console_w, map_console_h, 
                       TCODConsole::root, 0, 0);
    render_hud ();
    TCODConsole::blit (hud_console, 0, 0, hud_console_w, hud_console_h, 
                       TCODConsole::root, 80, 0);
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
