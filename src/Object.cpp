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

Object::Object (char new_c, const char* new_name, TCODColor new_color, 
                bool new_blocks, CSheet* new_csheet) {
  id = get_next_id ();
  c = new_c;
  name = (char*) malloc (1 + sizeof (char) * strlen (new_name));
  strcpy (name, new_name);
  color = new_color;
  blocks = new_blocks;
  csheet = new_csheet;
}

Object::~Object () {
  recycle_id ();
  delete (name);
  delete (csheet);
}

void Object::delete_registered_objects () {
  while (!object_registry->empty ()) {
    delete ((*(object_registry->begin ())).second);
  }
}

Object* Object::get_object_by_id (int target_id) {
  return ((*object_registry)[target_id]);
}
