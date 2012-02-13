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

class AI {
public:
  TCODMap* fov_map;
  Object* object;
  int in_pursuit;
  Coord last_seen;

  AI (Object* new_object);
  void init_fov_map ();
  list<Object*>* visible_objects ();
  void move_towards (const Coord& target);
  void move_towards_smarter (const Coord& target);
  Coord nearest_view_to_target (const Coord& loc);
  Coord nearest_view_to_target_from_src (const Coord& loc, const Coord& src);
  void take_turn ();
};

#endif
