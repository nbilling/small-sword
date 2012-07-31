#ifndef TYPES_HPP
#define TYPES_HPP

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
