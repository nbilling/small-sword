#include "Object.hpp"

int Object::next_id;
list<int>* Object::recycled_ids;
int Object::object_count;

int Object::get_next_id () {
  object_count++;

  if (!recycled_ids)
    recycled_ids = new list<int> ();

  if (recycled_ids->empty ()) {
    return (next_id++);
  }
  else {
    int retval = recycled_ids->front ();
    recycled_ids->pop_front ();
    return (retval);
  }
}

void Object::recycle_id () {
  object_count--;
  if (object_count == 0) {
    next_id = 0;
    delete (recycled_ids);
  }
  else
    recycled_ids->push_front (id);
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
