#ifndef TYPES_HPP
#define TYPES_HPP

class Zone;
class AI;
class ObjId;
class Object;
class Lifeform;
class Weapon;
class TacticalUI;
class Tile;
class Rect;

enum ObjType {
    LifeformType,
    WeaponType
};

typedef struct {
    int x;
    int y;
} Coord;

inline bool coord_eq (const Coord& a, const Coord& b) {
    return (a.x == b.x && a.y == b.y);
}

#endif
