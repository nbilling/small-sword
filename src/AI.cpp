#include "AI.hpp"
#include "iostream"

#define INF INT_MAX

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

void AI::move_towards_smarter (const Coord& target){
  int dir = pathfind_step_dijkstra (object->loc, target, object->zone);
  walk (object, dir);
}

// Return true if each Coord in path is visible from at least one of fov1/fov2
inline bool AI::path_in_fov (list<Coord>* path, const TCODMap* fov1, const TCODMap* fov2) {
  bool in_fov = true;
  for (list<Coord>::iterator c = path->begin (); c != path->end (); c++)
    in_fov = in_fov && (fov1->isInFov ((*c).x, (*c).y) || fov2->isInFov ((*c).x, (*c).y));
  return (in_fov);
}

// Find closest feasible destination to target, if you start at src
Coord AI::closest_dest_to_target (const Coord& src, const Coord& target, Zone* zone) {
  // 
  // 
  PathMap path_map = dijkstra (src, zone);

  TCODMap* target_fov_map = new TCODMap (80,45);
  target_fov_map->copy (fov_map);
  target_fov_map->computeFov (target.x, target.y, TORCH_RADIUS, FOV_LIGHT_WALLS);
  fov_map->computeFov (object->loc.x, object->loc.y, TORCH_RADIUS, FOV_LIGHT_WALLS);
  

  int ring = 0;
  Coord cur = target;
  Coord best = (Coord){-1,-1};
  int best_to_src = INF;
  int best_to_target = INF;

  // Process center of spiral first
  if (object->zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
    list<Coord>* cur_path = find_path (src, cur, path_map);
      if (path_in_fov (cur_path, fov_map, target_fov_map)) {
        int cur_to_src = path_map.d[cur.x][cur.y];
        int cur_to_target = distance_to (cur, target);
        if (cur_to_target <= best_to_target && cur_to_src < best_to_src) {
          best_to_src = cur_to_src;
          best_to_target = cur_to_target;
          best = cur;
        }
      }
      delete (cur_path);
  }


  // Move outwards in a spiral from target (iterating through rings)
  while (ring < TORCH_RADIUS) {

    // Rightwards segment of spiral ring
    for (int i=1; i <= 2*ring + 1 ; i++) {
      cur.x += 1;
      if (object->zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (src, cur, path_map);
        if (path_in_fov (cur_path, fov_map, target_fov_map)) {
          int cur_to_src = path_map.d[cur.x][cur.y];
          int cur_to_target = distance_to (cur, target);
        if (cur_to_target <= best_to_target && cur_to_src < best_to_src) {
            best_to_src = cur_to_src;
            best_to_target = cur_to_target;
            best = cur;
          }
        }
        delete (cur_path);
      }
    }

    // Upwards segment of spiral ring
    for (int i=1; i <= 2*ring + 1 ; i++) {
      cur.y += 1;
      if (object->zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (src, cur, path_map);
        if (path_in_fov (cur_path, fov_map, target_fov_map)) {
          int cur_to_src = path_map.d[cur.x][cur.y];
          int cur_to_target = distance_to (cur, target);
          if (cur_to_target <= best_to_target && cur_to_src < best_to_src) {
            best_to_src = cur_to_src;
            best_to_target = cur_to_target;
            best = cur;
          }
        }
        delete (cur_path);
      }
    }

    // Leftwards segment of spiral ring
    for (int i=1; i <= 2*ring + 2 ; i++) {
      cur.x -= 1;
      if (object->zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (src, cur, path_map);
        if (path_in_fov (cur_path, fov_map, target_fov_map)) {
          int cur_to_src = path_map.d[cur.x][cur.y];
          int cur_to_target = distance_to (cur, target);
          if (cur_to_target <= best_to_target && cur_to_src < best_to_src) {
            best_to_src = cur_to_src;
            best_to_target = cur_to_target;
            best = cur;
          }
        }
        delete (cur_path);
      }
    }
    // Downwards segment of spiral ring
    for (int i=1; i <= 2*ring + 2 ; i++) {
      cur.y -= 1;
      if (object->zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (src, cur, path_map);
        if (path_in_fov (cur_path, fov_map, target_fov_map)) {
          int cur_to_src = path_map.d[cur.x][cur.y];
          int cur_to_target = distance_to (cur, target);
          if (cur_to_target <= best_to_target && cur_to_src < best_to_src) {
            best_to_src = cur_to_src;
            best_to_target = cur_to_target;
            best = cur;
          }
        }
        delete (cur_path);
      }
    }

    ring++;
  }

  for (int i=0; i < zone->grid_w; i ++)
    delete path_map.d[i];
  delete path_map.d;
  for (int i=0; i < zone->grid_w; i++)
    delete path_map.p[i];
  delete path_map.p;
  delete (target_fov_map);

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
        Coord dest = closest_dest_to_target (object->loc, (*o)->loc, object->zone);
        if (dest.x >= 0 && dest.y >= 0)
          this->move_towards_smarter (dest);
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
    Coord dest = closest_dest_to_target (object->loc, last_seen, object->zone);
    if (distance_to (object, dest) >= 2) {
      this->move_towards_smarter (dest);
    }
  }
}

