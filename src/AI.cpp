#include "list"

#include "Rulebook.cpp"

#define FOV_ALGO 0  
#define FOV_LIGHT_WALLS true
#define TORCH_RADIUS 10

class AI {
public:
  TCODMap* fov_map;
  Object* object;

  AI (Object* new_object) {
    object = new_object;
  }

  void init_fov_map (Rulebook* rulebook) {
    for (int y=0; y < rulebook->zone->grid_h; y++) {
      for (int x=0; x < rulebook->zone->grid_w; x++) {
              fov_map->setProperties (x, y, !rulebook->zone->grid[x][y]->blocked, 
                                      !rulebook->zone->grid[x][y]->block_sight);
      }
    }
  }

  list<Object*>* visible_objects (Rulebook* rulebook) {
    list<Object*>* visible = new list<Object*>();
    for (list<Object*>::iterator o = rulebook->zone->objects->begin();
         o != rulebook->zone->objects->end(); o++) {
      if (fov_map->isInFov ((*o)->x, (*o)->y)) {
        visible->push_back (*o);
      }
    }
    return (visible);
  }

  void take_turn (Rulebook* rulebook) {
    if (fov_map == NULL) {
      fov_map = new TCODMap(rulebook->zone->grid_w, rulebook->zone->grid_h);
      init_fov_map(rulebook);
    }
        
    fov_map->computeFov(object->x, object->y, TORCH_RADIUS, FOV_LIGHT_WALLS);
    list<Object*>* visible = visible_objects(rulebook);                
    for (list<Object*>::iterator o = visible->begin(); 
         o != visible->end(); o++) {
      if (strcmp ((*o)->name, "player") == 0) {
        //move towards player if far away
        if (rulebook->distance_to(object, (*o)) >= 2) {
          rulebook->move_object_towards(object, (*o)->x, (*o)->y);
        }
        //close enough, attack! (if the player is still alive.)
        else if ((*o)->csheet->hp > 0) {
          rulebook->attack(object, (*o));
          break;
        }
      }
    }
  }
};                
