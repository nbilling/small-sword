#ifndef ZONE_HPP
#define ZONE_HPP

#include "cmath"
#include "list"
#include "map"
#include "Types.hpp"
#include "Tile.hpp"
#include "Object.hpp"
using namespace std;

#define color_wall TCODColor::grey
#define color_ground TCODColor::desaturatedOrange

class Zone{
    private:
        // Inhabitants
        map<ObjId,Coord>* object_locations;

        // Bookkeeping
        bool** blocked;

        // Terrain
        Tile::Tile*** grid;

        // Grid dimensions
        int grid_w;
        int grid_h;

    public:
        Zone (int new_grid_w, int new_grid_h);

        ~Zone ();

        void place_object (ObjId object_id, const Coord& loc);

        void move_object (ObjId object_id, const Coord& loc);

        bool is_blocked (const Coord& loc);

        bool in_bounds (const Coord& loc);

        list<ObjId>* objects_at (const Coord& loc);

        Coord location_of (ObjId object_id);

        int width ();

        int height ();

        void set_tile_char (const Coord& loc, char new_c);

        void set_tile_color (const Coord& loc, TCODColor new_color);

        void set_tile_blocked (const Coord& loc, bool new_blocked);

        void set_tile_block_sight (const Coord& loc, bool new_block_sight);

        void set_tile_explored (const Coord& loc, bool new_explored);

        TCODColor get_tile_color (const Coord& loc);

        bool get_tile_explored (const Coord& loc);

        TCODMap* new_fov_map ();

};

#endif
