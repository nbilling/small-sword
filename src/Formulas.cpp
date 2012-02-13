#include "Formulas.hpp"

int distance_to (Object* src_object, Object* dest_object) {
  float dx = (float) dest_object->loc.x - src_object->loc.x;
  float dy = (float) dest_object->loc.y - src_object->loc.y;    
  return ((int) sqrt (pow (dx, 2) + pow (dy, 2)));
}

int distance_to (Object* src_object, const Coord& dest) {
  float dx = (float) dest.x - src_object->loc.x;
  float dy = (float) dest.y - src_object->loc.y;    
  return ((int) sqrt (pow (dx, 2) + pow (dy, 2)));
}

int distance_to (const Coord& src, const Coord& dest) {
  float dx = (float) dest.x - src.x;
  float dy = (float) dest.y - src.y;    
  return ((int) sqrt (pow (dx, 2) + pow (dy, 2)));
}

int displacement_to_direction (int dx, int dy) {
  float distance = sqrt (pow ((float)dx, 2) + pow((float)dy, 2));

  //normalize it to length 1 (preserving direction), then round it and
  //convert to integer so the movement is restricted to the grid
  int move_x = round (dx / distance);
  int move_y = round (dy / distance);
  
  if (move_x == -1 && move_y == -1)
    return 7;
  else if (move_x == 0 && move_y == -1)
    return 8;
  else if (move_x == 1 && move_y == -1)
    return 9;
  else if (move_x == -1 && move_y == 0)
    return 4;
  else if (move_x == 0 && move_y == 0)
    return 5;
  else if (move_x == 1 && move_y == 0)
    return 6;
  else if (move_x == -1 && move_y == 1)
    return 1;
  else if (move_x == 0 && move_y == 1)
    return 2;
  else if (move_x == 1 && move_y == 1)
    return 3;

  return 0;
}
