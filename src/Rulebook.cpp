#include "cmath"

#include "Zone.cpp"

class Rulebook {
public:
  Zone* zone;

  Rulebook (Zone* new_zone) {
    zone = new_zone;
  }

  int distance_to (Object* src_object, Object* dest_object) {
    float dx = (float) dest_object->x - src_object->x;
    float dy = (float) dest_object->y - src_object->y;    
    return ((int) sqrt (pow (dx, 2) + pow (dy, 2)));
  }

  void move_object (Object* object, int dx, int dy) {
    if (!zone->is_blocked (object->x + dx, object->y + dy)) {
      object->move(dx, dy);
    }
  }

  void move_object_towards (Object* object, int target_x, int target_y){
    float dx = (float) (target_x - object->x);
    float dy = (float) (target_y - object->y);
    float distance = sqrt (pow (dx, 2) + pow(dy, 2));
 
    //normalize it to length 1 (preserving direction), then round it and
    //convert to integer so the movement is restricted to the grid
    int move_x = round (dx / distance);
    int move_y = round (dy / distance);
    move_object(object, move_x, move_y);
  }

  void attack (Object* src_object, Object* dest_object) {
    if (distance_to (src_object, dest_object) < 2) {
      dest_object->csheet->hp -=1;
    }
  }
};
