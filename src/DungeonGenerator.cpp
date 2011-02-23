//Standard library
#include "list"

//libtcod
#include "libtcod.hpp"
using namespace std;
//My files
#include "Rect.cpp"
#include "AI.cpp"

//Definitions
#define MAXINT(a,b) (a > b ? a : b)
#define MININT(a,b) (a < b ? a : b)

#define ROOM_MAX_SIZE 10
#define ROOM_MIN_SIZE 6
#define MAX_ROOMS 30
#define MAX_ROOM_MONSTERS 3

inline
void create_room (Zone* zone, const Rect* room) {
  //go through the tiles in the rectangle and make them passable
  for (int x=room->x1 + 1; x < room->x2; x++)
    for (int y=room->y1 + 1; y < room->y2; y++){
      zone->grid[x][y]->c = '.';
      zone->grid[x][y]->blocked = false;
      zone->grid[x][y]->block_sight = false;
    }
}
 
inline
void create_h_tunnel (Zone* zone, int x1, int x2, int y) {
  //horizontal tunnel. min() and max() are used in case x1>x2
  for (int x = MININT(x1, x2); x < MAXINT(x1, x2) + 1; x++){
    zone->grid[x][y]->c = '.';
    zone->grid[x][y]->blocked = false;
    zone->grid[x][y]->block_sight = false;
  }
}
 
inline
void create_v_tunnel (Zone* zone, int y1, int y2, int x) {
  //vertical tunnel 
  for (int  y = MININT(y1, y2); y < MAXINT(y1, y2) + 1; y++) {
    zone->grid[x][y]->c = '.';
    zone->grid[x][y]->blocked = false;
    zone->grid[x][y]->block_sight = false;
  }
}

void place_objects (Zone* zone, list<AI*>* ais, const Rect* room) {
  TCODRandom* rng = TCODRandom::getInstance();
  //choose random number of monsters
  int num_monsters = rng->getInt(0, MAX_ROOM_MONSTERS);
    
  for (int i=0; i < num_monsters; i++) {
    //choose random spot for this monster
    int x = rng->getInt(room->x1, room->x2);
    int y = rng->getInt(room->y1, room->y2);
 
    //only place it if the tile is not blocked
    if (!zone->is_blocked(x, y)) {
      Object* monster;
      if (rng->getInt(0, 100) < 80) {  //80% chance of getting an orc
        //create an orc
        monster = new Object(x, y, 'o', "orc", TCODColor::desaturatedGreen,
                             true, new CSheet(10));
      }
      else {
        //create a troll
        monster = new Object(x, y, 'T', "troll", TCODColor::darkerGreen,
                             true, new CSheet(15));
      }
      zone->objects->push_back(monster);
      AI* ai = new AI (monster);
      ais->push_back(ai);
    }
  }
}

void make_grid (Zone* zone, list<AI*>* ais) {
  TCODRandom* rng = TCODRandom::getInstance();
  list<Rect*>* rooms = new list<Rect*>();    
  int num_rooms = 0;        
  for (int r=0; r < MAX_ROOMS; r++) {
    //Generate Rect with random width, height and position
    int w = rng->getInt(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
    int h = rng->getInt(ROOM_MIN_SIZE, ROOM_MAX_SIZE);
    int x = rng->getInt(0, zone->grid_w - w - 1);
    int y = rng->getInt(0, zone->grid_h - h - 1);            
    Rect* new_room = new Rect(x, y, w, h);            

    //Run through the other rooms and see if they intersect with this one
    bool failed = false;
    for (list<Rect*>::iterator other_room = rooms->begin(); 
         other_room != rooms->end(); other_room++) {
      if (new_room->intersect(*other_room)) {
        failed = true;
        break;
      }
    }
    if (!failed) {        
      //No intersections so "paint" it to the grid's tiles
      create_room(zone, new_room); 
      //Add random contents to this room, such as monsters
      place_objects(zone, ais, new_room); 
      int new_x = new_room->center_x();
      int new_y = new_room->center_y(); 
      if (num_rooms > 0) {
        //Connect all rooms after the first to the previous room
        int prev_x = rooms->back()->center_x();
        int prev_y = rooms->back()->center_y();                     
        if (rng->getInt(0, 1) == 1) { 
          //50% chance to tunnel horizontally, then vertically
          create_h_tunnel(zone, prev_x, new_x, prev_y);
          create_v_tunnel(zone, prev_y, new_y, new_x);
        }
        else {
          //50% chance to tunnel vertically, then horizontally
          create_v_tunnel(zone, prev_y, new_y, prev_x);
          create_h_tunnel(zone, prev_x, new_x, new_y);
        }
      }
      //Finally, append the new room to the list        
      rooms->push_back(new_room);
      num_rooms += 1;
    }
  }
}