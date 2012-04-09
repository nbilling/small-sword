#ifndef TYPES_HPP
#define TYPES_HPP

class Zone;
class AI;
class Object;
class TacticalUI;
class Tile;
class Rect;
class CSheet;

typedef struct {
    int x;
    int y;
} Coord;

inline bool coord_eq (const Coord& a, const Coord& b) {
    return (a.x == b.x && a.y == b.y);
}

#endif
