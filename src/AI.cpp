#include "AI.hpp"
#include "iostream"

AI::AI (Object* new_object) {
  object = new_object;
  in_pursuit = false;
  last_seen = (Coord) {0,0};
}

void AI::init_fov_map () {
  for (int y=0; y < object->zone->grid_h; y++) {
    for (int x=0; x < object->zone->grid_w; x++) {
      fov_map->setProperties (x, y, !object->zone->grid[x][y]->blocked, 
                              !object->zone->grid[x][y]->block_sight);
    }
  }
}

list<Object*>* AI::visible_objects () {
  list<Object*>* visible = new list<Object*>();
  for (list<Object*>::iterator o = object->zone->objects->begin();
       o != object->zone->objects->end(); o++) {
    if (fov_map->isInFov ((*o)->loc.x, (*o)->loc.y)) {
      visible->push_back (*o);
    }
  }
  return (visible);
}

void AI::move_towards (const Coord& target){
  float dx = (float) (target.x - object->loc.x);
  float dy = (float) (target.y - object->loc.y);
  float distance = sqrt (pow (dx, 2) + pow(dy, 2));
  
  //normalize it to length 1 (preserving direction), then round it and
  //convert to integer so the movement is restricted to the grid
  Coord move = (Coord) {round (dx / distance), round (dy / distance)};
  walk (object, move.x, move.y);
}

void AI::move_towards_smarter (const Coord& target){
  int dir = pathfind_step_dijkstra (object->loc, target, object->zone);
  walk (object, dir);
}

Coord AI::nearest_view_to_target (const Coord& loc) {
  // This function returns the nearest coordinate from
  // which you can see loc, since I'm pretty sure sight
  // is symmetric, we decide if you can see loc by checking
  // if someone at loc could see you.

  // If there is no such coordinate then return (-1,-1).
  int dist = 0;
  Coord cur = loc; 
  fov_map->computeFov (loc.x, loc.y, TORCH_RADIUS, FOV_LIGHT_WALLS);

  while (dist < TORCH_RADIUS) {
    for (int i=1; i <= 2*dist + 1 ; i++) {
      cur.x += 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y)))
          return cur;
    }
    for (int i=1; i <= 2*dist + 1; i++) {
      cur.y += 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y)))
          return cur;
    }
    for (int i=1; i <= 2*dist + 2; i++) {
      cur.x -= 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y)))
          return cur;
    }
    for (int i=1; i <= 2*dist + 2; i++) {
      cur.y -= 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y)))
          return cur;
    }
  }
  return ((Coord){-1,-1});
}

Coord AI::nearest_view_to_target_from_src (const Coord& loc, const Coord& src) {
  // Varies from nearest_view_to_target_from_src in that
  // it looks for the coordinate closest to some src.
  int ring = 0;
  int best_to_src = 10000; //Should be max int, but blah
  int best_to_loc = 10000; //Should be max int, but blah
  Coord cur = loc;
  Coord best = (Coord){-1,-1};
  fov_map->computeFov (loc.x, loc.y, TORCH_RADIUS, FOV_LIGHT_WALLS);


  // Move outwards in a spiral from loc (iterating through rings)
  while (ring < TORCH_RADIUS && best_to_src == 10000) {

    // Rightwards segment of spiral ring
    for (int i=1; i <= 2*ring + 1 ; i++) {
      cur.x += 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y))) {
          int cur_to_src = distance_to (cur, src);
          int cur_to_loc = distance_to (cur, loc);
          if (cur_to_loc < best_to_loc 
              || cur_to_loc == best_to_loc && cur_to_src < best_to_src) {
            best_to_loc = distance_to (cur, loc);
            best_to_src = distance_to (cur, src);
            best = cur;
          }
        }
    }

    // Upwards segment of spiral ring
    for (int i=1; i <= 2*ring + 1 ; i++) {
      cur.y += 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y))) {
          int cur_to_src = distance_to (cur, src);
          int cur_to_loc = distance_to (cur, loc);
          if (cur_to_loc < best_to_loc 
              || cur_to_loc == best_to_loc && cur_to_src < best_to_src) {
            best_to_loc = distance_to (cur, loc);
            best_to_src = distance_to (cur, src);
            best = cur;
          }
        }
    }

    // Leftwards segment of spiral ring
    for (int i=1; i <= 2*ring + 2 ; i++) {
      cur.x -= 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y))) {
          int cur_to_src = distance_to (cur, src);
          int cur_to_loc = distance_to (cur, loc);
          if (cur_to_loc < best_to_loc 
              || cur_to_loc == best_to_loc && cur_to_src < best_to_src) {
            best_to_loc = distance_to (cur, loc);
            best_to_src = distance_to (cur, src);
            best = cur;
          }
        }
    }
    // Downwards segment of spiral ring
    for (int i=1; i <= 2*ring + 2 ; i++) {
      cur.y -= 1;
      if (object->zone->in_bounds (cur))
        if ((!object->zone->is_blocked (cur)) 
            && (fov_map->isInFov (cur.x, cur.y))) {
          int cur_to_src = distance_to (cur, src);
          int cur_to_loc = distance_to (cur, loc);
          if (cur_to_loc < best_to_loc 
              || cur_to_loc == best_to_loc && cur_to_src < best_to_src) {
            best_to_loc = distance_to (cur, loc);
            best_to_src = distance_to (cur, src);
            best = cur;
          }
        }
    }

    ring++;
  }

  return (best);
}

void AI::take_turn () {
  if (fov_map == NULL) {
    fov_map = new TCODMap(object->zone->grid_w, object->zone->grid_h);
    init_fov_map();
  }
        
  fov_map->computeFov(object->loc.x, object->loc.y, TORCH_RADIUS, FOV_LIGHT_WALLS);
  list<Object*>* visible = visible_objects();                
  int player_spotted = false;
  for (list<Object*>::iterator o = visible->begin(); 
       o != visible->end(); o++) {
    if (strcmp ((*o)->name, "player") == 0) {
      player_spotted = true;
      in_pursuit = true;
      last_seen = (*o)->loc;

      //move towards player if far away
      if (distance_to (object, (*o)) >= 2) {
        Coord target = nearest_view_to_target_from_src ((*o)->loc, object->loc);
        if (target.x >= 0 && target.y >= 0)
          this->move_towards_smarter (target);
        break;
      }
      //close enough, attack! (if the player is still alive.)
      else if ((*o)->csheet->hp > 0) {
        attack (object, (*o));
        break;
      }
    }
  }

  if (!player_spotted && in_pursuit) {
    Coord target = nearest_view_to_target_from_src (last_seen, object->loc);
    if (distance_to (object, target) >= 2) {
      this->move_towards_smarter (target);
    }
  }
}

