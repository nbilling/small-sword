#ifndef DUNGEONGENERATOR_HPP
#define DUNGEONGENERATOR_HPP

#include "list"
#include "libtcod.hpp"
#include "Types.hpp"
#include "Rect.hpp"
#include "AI.hpp"

//Definitions
#define MAXINT(a,b) (a > b ? a : b)
#define MININT(a,b) (a < b ? a : b)

#define ROOM_MAX_SIZE 10
#define ROOM_MIN_SIZE 6
#define MAX_ROOMS 30
#define MAX_ROOM_MONSTERS 3

inline
void create_room (Zone* zone, const Rect* room);
 
inline
void create_h_tunnel (Zone* zone, int x1, int x2, int y);
 
inline
void create_v_tunnel (Zone* zone, int y1, int y2, int x);

void place_objects (Zone* zone, list<AI*>* ais, const Rect* room);

void make_grid (Zone* zone, map<int,Object*>* object_registry, list<AI*>* ais);

#endif
