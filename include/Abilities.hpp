#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include <assert.h>
#include "Types.hpp"
#include "Formulas.hpp"
#include "Object.hpp"

class AbilityInvocation {
    protected:
        int obj_id;
        Zone* zone;
    public:
        virtual void execute () {};
};

class WalkInvocation : public AbilityInvocation {
    private:
        list <Coord>* path;
    public:
        WalkInvocation (int new_obj_id, Zone* new_zone, list<Coord>* new_path);
        ~WalkInvocation ();
        void execute ();
};

class StepInvocation : public AbilityInvocation {
    private:
        int direction;
    public:
        StepInvocation (int new_obj_id, Zone* new_zone, int new_direction);
        ~StepInvocation ();
        void execute ();
};

class NullInvocation : public AbilityInvocation {
    public:
        NullInvocation (int new_obj_id, Zone* zone);
        ~NullInvocation ();
        void execute ();
};

class AttackInvocation : public AbilityInvocation {
    private:
        int target_obj_id;
    public:
        AttackInvocation (int new_obj_id, Zone* new_zone, int new_target_obj_id);
        ~AttackInvocation ();
        void execute ();
};

#endif
