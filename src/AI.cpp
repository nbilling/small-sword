#include "AI.hpp"
#include "iostream"

AI::AI (Object* new_object, Zone* new_zone) {
  object = new_object;
  zone = new_zone;
  fov_map = zone->new_fov_map ();
  in_pursuit = false;
  last_seen = (Coord) {0,0};
}

map<int,Coord>* AI::visible_objects () {
  map<int,Coord>* retval = new map<int,Coord> ();
  for (int i = 0; i < zone->width (); i ++)
    for (int j = 0; j < zone->height (); j++)
      if (fov_map->isInFov (i,j)) {
        list<int>* temp = zone->objects_at ((Coord){i,j});
        for (list<int>::iterator it = temp->begin ();
             it != temp->end (); it++)
          (*retval)[*it] = (Coord){i,j};
        delete (temp);
      }
  return (retval);
}

// Return true if each Coord in path is visible from at least one of fov1/fov2
inline bool AI::path_in_fov (list<Coord>* path, const TCODMap* fov1, 
                             const TCODMap* fov2) {
  bool in_fov = true;
  for (list<Coord>::iterator c = path->begin (); c != path->end (); c++)
    in_fov = in_fov && (fov1->isInFov ((*c).x, (*c).y) 
                        || fov2->isInFov ((*c).x, (*c).y));
  return (in_fov);
}

// Find closest feasible destination to target, if you start at src
Coord AI::closest_dest_to_target (const Coord& target, const PathMap& path_map) {

  TCODMap* target_fov_map = new TCODMap (80,45);
  target_fov_map->copy (fov_map);
  target_fov_map->computeFov (target.x, target.y, TORCH_RADIUS, FOV_LIGHT_WALLS);
  fov_map->computeFov (object_loc.x, object_loc.y, TORCH_RADIUS, FOV_LIGHT_WALLS);
  

  int ring = 0;
  Coord cur = target;
  Coord best = (Coord){-1,-1};
  int best_to_src = INF;
  int best_to_target = INF;

  // Process center of spiral first
  if (zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
    list<Coord>* cur_path = find_path (object_loc, cur, path_map);
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
      if (zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (object_loc, cur, path_map);
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
      if (zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (object_loc, cur, path_map);
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
      if (zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (object_loc, cur, path_map);
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
      if (zone->in_bounds (cur) && fov_map->isInFov (cur.x, cur.y)) {
        list<Coord>* cur_path = find_path (object_loc, cur, path_map);
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

  delete (target_fov_map);

  return (best);
}

StepInvocation* AI::approach (const Coord& target) {
  // calculate path_map
  PathMap path_map = dijkstra (object_loc, zone);

  // calculate dest from path_map
  Coord dest = closest_dest_to_target (target, path_map);
  assert (zone->in_bounds (dest));

  // calculate first step from path_map and dest
  // move
  list<Coord>* path = find_path (object_loc, dest, path_map);
  int dir;
  if (path->size () > 1) {
    Coord step_start = path->front ();
    path->pop_front ();
    Coord step_end = path->front ();
    path->pop_front ();
    dir = displacement_to_direction 
      (step_end.x - step_start.x, 
       step_end.y - step_start.y);
  }
  else
    dir = 5;

  delete path;
  for (int i=0; i < zone->width (); i ++)
    delete path_map.d[i];
  delete path_map.d;
  for (int i=0; i < zone->width (); i++)
    delete path_map.p[i];
  delete path_map.p;

  return (new StepInvocation (object->id, zone, dir));
}

AbilityInvocation* AI::take_turn () {
  object_loc = zone->location_of (object->id);
  fov_map->computeFov(object_loc.x, object_loc.y, TORCH_RADIUS, FOV_LIGHT_WALLS);
  map<int,Coord>* visible = visible_objects ();
  bool player_spotted = false;

  for (map<int,Coord>::iterator it = visible->begin ();
       it != visible->end(); it++) {
    Object* o = Object::get_object_by_id ((*it).first);
    Coord o_loc = (*it).second;
    if (strcmp (o->name, "player") == 0) {
      player_spotted = true;
      in_pursuit = true;
      last_seen = o_loc;

      //move towards player if far away
      if (distance_to (object_loc, o_loc) >= 2) {
        delete (visible);
        return (approach (last_seen));
      }
      //close enough, attack! (if the player is still alive.)
      else if (o->csheet->hp > 0) {
        delete (visible);
        return (new AttackInvocation (object->id, zone, o->id));
      }
    }
  }

  if (!player_spotted && in_pursuit) {
    if (coord_eq (object_loc, last_seen))
      in_pursuit = false;
    else {
      delete (visible);
      return (approach (last_seen));
    }
  }

  delete (visible);
  return (new NullInvocation (object->id, zone));
}

