#include "TacticalUI.hpp"


TacticalUI::TacticalUI (Zone* new_zone, list<AI*>* new_ais, Object* new_player, TCODMap* new_fov_map, 
                        TCODConsole* new_map_console, int new_map_console_w, int new_map_console_h, 
                        TCODConsole* new_hud_console, int new_hud_console_w, int new_hud_console_h) {
  zone = new_zone;
  ais = new_ais;
  player = new_player;
  fov_map = new_fov_map;
  map_console = new_map_console;
  map_console_w = new_map_console_w;
  map_console_h = new_map_console_h;
  hud_console = new_hud_console;
  hud_console_w = new_hud_console_w;
  hud_console_h = new_hud_console_h;
}
  
void TacticalUI::render_grid () {
  //Draw the grid 
  for (int y=0; y < zone->grid_h; y++){
    for (int x=0; x < zone->grid_w; x++){
      int visible = fov_map->isInFov (x, y);
      int wall = zone->grid[x][y]->block_sight;
      if (!visible){
        if (zone->grid[x][y]->explored) {
          if (wall) {
            map_console->setCharBackground (x, y, color_dark_wall);
          }
          else {
            map_console->setCharBackground (x, y, color_dark_ground);
          }
        }
      }
      else {
        if (wall) {
          map_console->setCharBackground (x, y, color_light_wall);
        }
        else {
          map_console->setCharBackground (x, y, color_light_ground);
        }
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
      if (fov_map->isInFov (i,j)) {
        list<int>* temp = zone->objects_at ((Coord){i,j});
        if (!temp->empty ()) {
          Object* o = Object::get_object_by_id (temp->front ());
          //Set the color and then draw the character for this object
          map_console->setCharForeground (i, j, o->color);
          map_console->putChar (i, j, o->c, TCOD_BKGND_NONE);
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
      list<int>* temp = zone->objects_at ((Coord){i,j});
      if (!temp->empty ()) {
        //Erase the character for this object
        map_console->putChar (i, j, ' ', TCOD_BKGND_NONE);
      }
      delete (temp);
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

int TacticalUI::handle_keys () {
  TCOD_key_t key = {TCODK_NONE,0};
  TCOD_mouse_t mouse;

  if (!zone) 
    cout << "null zone in handle_keys" << endl;

  //    TCOD_key_t key = TCODConsole::waitForKeypress (true);
  TCODSystem::waitForEvent((TCOD_event_t)(TCOD_EVENT_KEY_PRESS|TCOD_EVENT_MOUSE),&key,&mouse, false);

  if (key.vk == TCODK_ENTER && key.lalt) {
    TCODConsole::setFullscreen (!TCODConsole::isFullscreen());
    return (0);
  } 
  else if (key.vk == TCODK_ESCAPE) {
    return (-1);
  }

  if (game_state == GS_PLAYING) {
    //Movement keys
    if (key.vk == TCODK_CHAR && key.c == 'k') {
      walk (player, zone, 0, -1);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c =='j') {
      walk (player, zone, 0, 1);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'h') {
      walk (player, zone, -1, 0);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'l') {
      walk (player, zone, 1, 0);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'y') {
      walk (player, zone, -1, -1);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'u') {
      walk (player, zone, 1, -1);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'b') {
      walk (player, zone, -1, 1);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'n') {
      walk (player, zone, 1, 1);
      fov_recompute = true;
    }
    else if (key.vk == TCODK_CHAR && key.c == 'x') {
      // Call function to do targetting.
    }
  }    
  return (0);
}
  
int TacticalUI::display () {
  fov_recompute = true;
  game_state = GS_PLAYING;
  player_action = PA_NONE;

  while (!TCODConsole::isWindowClosed ()) {
    if (fov_recompute) {
      fov_recompute = false;
      Coord player_loc = zone->location_of (player->id);
      fov_map->computeFov (player_loc.x, player_loc.y, TORCH_RADIUS, 
                           FOV_LIGHT_WALLS); 
    }
    render_grid ();
    render_objects ();

    TCODConsole::blit (map_console, 0, 0, map_console_w, map_console_h, 
                       TCODConsole::root, 0, 0);
    render_hud ();
    TCODConsole::blit (hud_console, 0, 0, hud_console_w, hud_console_h, 
                       TCODConsole::root, 80, 0);
    TCODConsole::flush ();
    clear_objects ();
      
    if (handle_keys () == -1) {
      break;
    }

    for (list<AI*>::iterator ai = ais->begin();
         ai != ais->end(); ai++){
      (*ai)->take_turn();
    }    
  }
  return (0);
}
