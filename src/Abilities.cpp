#include "Abilities.hpp"
#include "iostream"
#include "sstream"

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
            cerr << "Abilities => walk => attempt to walk out of bounds"
                << endl;
            break;
        }
        if (zone->is_blocked (*it))
            break;
        zone->move_object (obj_id, *it);
    }
}

const string WalkInvocation::message () {
    Object* obj = Object::get_object_by_id (obj_id);
    if (path->empty ()) {
        return (obj->get_name () + " is moving nowhere.");
    }
    else {
        ostringstream ss;
        ss << obj->get_name () << " is moving to (" << (path->front ()).x << ","
            << (path->front ()).y << ").";
        return (ss.str ());
    }
}

// StepInvocation methods
StepInvocation::StepInvocation (ObjId new_obj_id, Zone* new_zone,
        int new_direction) {
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

const string StepInvocation::message() {
    string dir;
    switch (direction) {
        case 1:
            dir = "SW";
            break;
        case 2:
            dir = "S";
            break;
        case 3:
            dir = "SE";
            break;
        case 4:
            dir = "W";
            break;
        case 5:
            dir = "nowhere";
            break;
        case 6:
            dir = "E";
            break;
        case 7:
            dir = "NW";
            break;
        case 8:
            dir = "N";
            break;
        case 9:
            dir = "NE";
            break;
    }
    Object* obj = Object::get_object_by_id (obj_id);
    ostringstream ss;
    ss << obj->get_name () << " is moving " << dir << ".";
    return (ss.str ());
}

// NullInvocation methods
NullInvocation::NullInvocation (ObjId new_obj_id, Zone* new_zone) {
    obj_id = new_obj_id;
    zone = new_zone;
}

NullInvocation::~NullInvocation () {}

void NullInvocation::execute () {}

const string NullInvocation::message () {
    return ("nothing");
}

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
    ObjId R_id = ((Lifeform*) Object::get_object_by_id
            (obj_id))->get_equipped_right_hand ();
    if (R_id) {
        Object* R_object = Object::get_object_by_id (R_id);
        if (R_object->type () == WeaponType)
            if (distance_to (zone->location_of (obj_id),
                        zone->location_of (target_obj_id))
                    <= ((Weapon*) R_object)->get_swing_range ())
                target->set_hp (target->get_hp ()
                     - ((Weapon*) R_object)->get_swing_damage ());
    }
    else
        target->set_hp (target->get_hp () - 1);
    if (target->get_hp () <= 0) {
        target->set_char ('%');
        target->set_color(TCODColor::red);
        target->set_blocks (false);
        zone->refresh_object (target->get_id ());
    }
}

const string AttackInvocation::message () {
    Object* obj = Object::get_object_by_id (obj_id);
    Object* target_obj = Object::get_object_by_id (target_obj_id);
    return (obj->get_name () + " is attacking " + target_obj->get_name () +
            ".");
}
