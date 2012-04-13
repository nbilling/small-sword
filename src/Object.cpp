#include "Object.hpp"

ObjId::ObjId (int new_id) {
    id = new_id;
}

ObjId& ObjId::operator++ () {
    id++;
    return (*this);
}

ObjId& ObjId::operator= (const ObjId& rhs) {
    if (this != &rhs)
        id = rhs.id;
    return (*this);
}

ObjId& ObjId::operator= (int new_id) {
    id = new_id;
    return (*this);
}

bool ObjId::operator< (const ObjId& rhs) const {
    return (id < rhs.id);
}

bool ObjId::operator! () {
    return (!id);
}

ObjId::operator bool () {
    return ((bool) id);
}

ObjId Object::next_id;
list<ObjId>* Object::recycled_ids = new list<ObjId> ();
int Object::object_count;
map<ObjId, Object*>* Object::object_registry = new map<ObjId,Object*> ();

ObjId Object::get_next_id () {
    object_count++;

    if (recycled_ids->empty ()) {
        ++next_id;
        (*object_registry)[next_id] = this;
        return (next_id);
    }
    else {
        ObjId retval = recycled_ids->front ();
        recycled_ids->pop_front ();
        (*object_registry)[retval] = this;
        return (retval);
    }
}

void Object::recycle_id () {
    object_count--;
    if (object_count == 0) {
        object_registry->erase (id);
        next_id = 0;
    }
    else {
        recycled_ids->push_front (id);
        object_registry->erase (id);
    }
}

void Object::delete_registered_objects () {
    while (!object_registry->empty ()) {
        delete ((*(object_registry->begin ())).second);
    }
}

Object* Object::get_object_by_id (ObjId target_id) {
    assert (target_id);
    return ((*object_registry)[target_id]);
}

ObjId Object::get_id () {
    return (id);
}

char Object::get_char () {
    return (c);
}

char* Object::get_name () {
    char* temp = new char[strlen (name)];
    strcpy (temp, name);
    return (temp);
}

TCODColor Object::get_color () {
    return (color);
}

bool Object::get_blocks () {
    return (blocks);
}

Lifeform::Lifeform (char new_c, const char* new_name, TCODColor new_color,
        bool new_blocks, int new_max_hp) {
    id = get_next_id ();
    c = new_c;
    name = new char[strlen (new_name)];
    strcpy (name, new_name);
    color = new_color;
    blocks = new_blocks;

    max_hp = new_max_hp;
    hp = max_hp;
    equipment.back = 0;
    equipment.right_hand = 0;
}

Lifeform::~Lifeform () {
    recycle_id ();
    delete (name);
}

ObjType Lifeform::type () {
    return (LifeformType);
}

int Lifeform::get_max_hp () {
    return (max_hp);
}

int Lifeform::get_hp () {
    return (hp);
}

ObjId Lifeform::get_equipped_back () {
    return (equipment.back);
}

ObjId Lifeform::get_equipped_right_hand () {
    return (equipment.right_hand);
}

void Lifeform::set_max_hp (int new_max_hp) {
    max_hp = new_max_hp;
}

void Lifeform::set_hp (int new_hp) {
    hp = new_hp;
}

void Lifeform::equip_back (ObjId obj_id) {
    equipment.back = obj_id;
}

void Lifeform::equip_right_hand (ObjId obj_id) {
    equipment.right_hand = obj_id;
}

Weapon::Weapon (char new_c, const char* new_name, TCODColor new_color,
        bool new_blocks, bool new_swingable, int new_swing_damage,
        int new_swing_range) {
    id = get_next_id ();
    c = new_c;
    name = new char[strlen (new_name)];
    strcpy (name, new_name);
    color = new_color;
    blocks = new_blocks;

    swingable = new_swingable;
    swing_damage = new_swingable;
    swing_range = new_swing_range;
}

Weapon::~Weapon () {
    recycle_id ();
    delete (name);
}

ObjType Weapon::type () {
    return (WeaponType);
}

bool Weapon::get_swingable () {
    return (swingable);
}

int Weapon::get_swing_damage () {
    return (swing_damage);
}

int Weapon::get_swing_range () {
    return (swing_range);
}
