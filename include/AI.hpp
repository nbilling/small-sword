#ifndef AI_HPP
#define AI_HPP

class AI;

#include "assert.h"
#include "iostream"
#include "list"

#include "libtcod.hpp"

#include "Abilities.hpp"
#include "Formulas.hpp"
#include "Object.hpp"
#include "Pathfinding.hpp"
#include "Types.hpp"

using namespace std;

#define FOV_ALGO 0
#define FOV_LIGHT_WALLS true
#define TORCH_RADIUS 10
#define INF INT_MAX

class AI {
    public:
        AI (Object* new_object, Zone* zone);
        AbilityInvocation* take_turn ();
        bool is_dead ();

    private:
        Zone* zone;
        TCODMap* fov_map;

        Object* object;
        // FIXME: This is just used as a global variable in take_turn, it is not kept up-to-date in between take_turn invocations. Cleanup?
        Coord object_loc;

        bool in_pursuit;
        Coord last_seen;

        void init_fov_map ();
        map<ObjId,Coord>* visible_objects ();
        Coord closest_dest_to_target (const Coord& target, const PathMap& path_map);
        StepInvocation* approach (const Coord& target);
        inline bool path_in_fov (list<Coord>* path, const TCODMap* fov1,
                const TCODMap* fov2);

};

#endif
