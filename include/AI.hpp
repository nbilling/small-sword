#ifndef AI_HPP
#define AI_HPP

#include "list"
#include "libtcod.hpp"
#include "Types.hpp"
#include "Formulas.hpp"
#include "Abilities.hpp"
#include "Object.hpp"
#include "Pathfinding.hpp"


#define FOV_ALGO 0  
#define FOV_LIGHT_WALLS true
#define TORCH_RADIUS 10
#define INF INT_MAX

class AI {
public:
  TCODMap* fov_map;
  Zone* zone;

  Object* object;
  Coord object_loc;

  int in_pursuit;
  Coord last_seen;

  AI (Object* new_object, const Coord& loc, Zone* zone);
  void init_fov_map ();
  map<int,Coord>* visible_objects ();
  void take_turn ();
  Coord closest_dest_to_target (const Coord& target, const PathMap& path_map);
  void approach (const Coord& target);

private:
  inline bool path_in_fov (list<Coord>* path, const TCODMap* fov1, const TCODMap* fov2);

};

#endif
