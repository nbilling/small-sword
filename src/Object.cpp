#include "Object.hpp"

int Object::next_id;
list<int>* Object::recycled_ids = new list<int> ();
int Object::object_count;
map<int, Object*>* Object::object_registry = new map<int,Object*> ();

int Object::get_next_id () {
    object_count++;

    if (recycled_ids->empty ()) {
        next_id++;
        (*object_registry)[next_id] = this;
        return (next_id);
    }
    else {
        int retval = recycled_ids->front ();
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

Object* Object::get_object_by_id (int target_id) {
    return ((*object_registry)[target_id]);
}

int Object::get_id () {
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
}

Lifeform::~Lifeform () {
    recycle_id ();
    delete (name);
}
int Lifeform::get_max_hp () {
    return (max_hp);
}

int Lifeform::get_hp () {
    return (hp);
}

void Lifeform::set_max_hp (int new_max_hp) {
    max_hp = new_max_hp;
}

void Lifeform::set_hp (int new_hp) {
    hp = new_hp;
}
