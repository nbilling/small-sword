#include "Zone.hpp"

Zone::Zone (int new_grid_w, int new_grid_h){
    object_locations = new map<ObjId,Coord> ();

    grid = new Tile**[new_grid_w];
    for (int i=0; i < new_grid_w; i++){
        grid[i] = new Tile*[new_grid_h];
        for (int j=0; j < new_grid_h; j++){
            grid[i][j] = new Tile::Tile(' ', color_wall, true, true);
        }
    }
    grid_w = new_grid_w;
    grid_h = new_grid_h;

    blocked = new bool*[new_grid_w];
    for (int i=0; i < new_grid_w; i++) {
        blocked[i] = new bool[new_grid_h];
        for (int j=0; j < new_grid_h; j++)
            blocked[i][j] = true;
    }
}

Zone::~Zone () {
    delete (object_locations);
    for (int i=0; i < grid_w; i++)
        delete (blocked[i]);
    delete (blocked);

    for (int i=0; i < grid_w; i++) {
        for (int j=0; j < grid_h; j++)
            delete (grid[i][j]);
        delete (grid[i]);
    }
    delete (grid);
}

// Place object given by object_id at loc. Do nothing if loc is blocked.
// If you call this on an object already in the zone then it will break
// the zone.
void Zone::place_object (ObjId object_id,  const Coord& loc) {
    if (blocked[loc.x][loc.y])
        return;
    (*object_locations)[object_id] = loc;
    blocked[loc.x][loc.y] = grid[loc.x][loc.y]->blocked
        || (Object::get_object_by_id (object_id))->get_blocks ();
}

// Move object given by object_id to loc. Do nothing if loc is blocked.
// If you call this on an object not already in the zone then it will
// break the zone.
void Zone::move_object (ObjId object_id,  const Coord& loc) {
    if (blocked[loc.x][loc.y])
        return;
    Coord old_loc = (*object_locations)[object_id];
    blocked[old_loc.x][old_loc.y] = grid[old_loc.x][old_loc.y]->blocked;
    (*object_locations)[object_id] = loc;
    blocked[loc.x][loc.y] = grid[loc.x][loc.y]->blocked
        || (Object::get_object_by_id (object_id))->get_blocks ();
}

void Zone::remove_object (ObjId id) {
    assert (id);
    object_locations->erase (id);
}

void Zone::refresh_object (ObjId id) {
    Coord loc = location_of (id);
    blocked[loc.x][loc.y] = grid[loc.x][loc.y]->blocked
        || (Object::get_object_by_id (id))->get_blocks ();
}

bool Zone::is_blocked (const Coord& loc) {
    return (blocked[loc.x][loc.y]);
}

bool Zone::in_bounds (const Coord& loc) {
    return (0 <= loc.x && loc.x < grid_w && 0 <= loc.y && loc.y < grid_h);
}

list<ObjId>* Zone::objects_at (const Coord& loc) {
    list<ObjId>* retval = new list<ObjId> ();
    for (map<ObjId,Coord>::iterator it = object_locations->begin ();
            it != object_locations->end (); it++) {
        if (coord_eq ((*it).second, loc)) {
            retval->push_front ((*it).first);
        }
    }
    return (retval);
}

Coord Zone::location_of (ObjId object_id) {
    return ((*object_locations)[object_id]);
}

int Zone::width () {
    return (grid_w);
}

int Zone::height () {
    return (grid_h);
}

void Zone::set_tile_char (const Coord& loc, char new_c) {
    grid[loc.x][loc.y]->c = new_c;
}

void Zone::set_tile_color (const Coord& loc, TCODColor new_color) {
    grid[loc.x][loc.y]->color = new_color;
}

void Zone::set_tile_blocked (const Coord& loc, bool new_blocked) {
    grid[loc.x][loc.y]->blocked = new_blocked;
    bool object_blocked = ! ((objects_at (loc))->empty ());
    blocked[loc.x][loc.y] = new_blocked || object_blocked;
}

void Zone::set_tile_block_sight (const Coord& loc, bool new_blocks_sight) {
    grid[loc.x][loc.y]->block_sight = new_blocks_sight;
}

void Zone::set_tile_explored (const Coord& loc, bool new_explored) {
    grid[loc.x][loc.y]->explored = new_explored;
}

TCODColor Zone::get_tile_color (const Coord& loc) {
    return (grid[loc.x][loc.y]->color);
}

bool Zone::get_tile_explored (const Coord& loc) {
    return (grid[loc.x][loc.y]->explored);
}

TCODMap* Zone::new_fov_map () {
    TCODMap* fov_map = new TCODMap::TCODMap (grid_w, grid_h);
    for (int i=0; i < grid_w; i++)
        for (int j=0; j < grid_h; j++) {
            fov_map->setProperties (i, j, !grid[i][j]->blocked,
                    !grid[i][j]->block_sight);
        }
    return fov_map;
}
