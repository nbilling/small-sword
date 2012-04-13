#include "Abilities.hpp"
#include "iostream"

// WalkInvocation methods
WalkInvocation::WalkInvocation (ObjId new_obj_id, Zone* new_zone, 
        list<Coord>* new_path) {
    obj_id = new_obj_id;
    zone = new_zone;
    path = new_path;
}

WalkInvocation::~WalkInvocation (){
    delete path;
}

void WalkInvocation::execute () {
    for (list<Coord>::iterator it = path->begin (); 
            it != path->end (); it++) {
        if (!zone->in_bounds (*it)) {
            cerr << "Abilities => walk => attempt to walk out of bounds" << endl;
            break;
        }
        if (zone->is_blocked (*it))
            break;
        zone->move_object (obj_id, *it);
    }
}

// StepInvocation methods
StepInvocation::StepInvocation (ObjId new_obj_id, Zone* new_zone, int new_direction) {
    obj_id = new_obj_id;
    zone = new_zone;
    direction = new_direction;
}

StepInvocation::~StepInvocation () {}

void StepInvocation::execute () {
    Coord new_loc = zone->location_of (obj_id);
    switch (direction) {
        case 1:
            new_loc.x += -1;
            new_loc.y += 1;
            break;
        case 2:
            new_loc.x += 0;
            new_loc.y += 1;
            break;
        case 3:
            new_loc.x += 1;
            new_loc.y += 1;
            break;
        case 4:
            new_loc.x += -1;
            new_loc.y += 0;
            break;
        case 5:
            new_loc.x += 0;
            new_loc.y += 0;
            break;
        case 6:
            new_loc.x += 1;
            new_loc.y += 0;
            break;
        case 7:
            new_loc.x += -1;
            new_loc.y += -1;
            break;
        case 8:
            new_loc.x += 0;
            new_loc.y += -1;
            break;
        case 9:
            new_loc.x += 1;
            new_loc.y += -1;
            break;
    }
    assert (zone->in_bounds (new_loc));
    // Should handle case where new_loc is blocked
    zone->move_object (obj_id, new_loc);
}

// NullInvocation methods
NullInvocation::NullInvocation (ObjId new_obj_id, Zone* new_zone) {
    obj_id = new_obj_id;
    zone = new_zone;
}

NullInvocation::~NullInvocation () {}

void NullInvocation::execute () {}

// AttackInvocation methods
AttackInvocation::AttackInvocation (ObjId new_obj_id, Zone* new_zone, 
        ObjId new_target_obj_id) {
    obj_id = new_obj_id;
    zone = new_zone;
    target_obj_id = new_target_obj_id;
}

AttackInvocation::~AttackInvocation () {}

void AttackInvocation::execute () {
    Lifeform* target = (Lifeform*) Object::get_object_by_id (target_obj_id);
    target->set_hp (target->get_hp () - 1);
}
