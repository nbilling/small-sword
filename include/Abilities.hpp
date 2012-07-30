#ifndef ABILITIES_HPP
#define ABILITIES_HPP

#include <assert.h>
#include "Types.hpp"
#include "Formulas.hpp"
#include "Object.hpp"

class AbilityInvocation {
    protected:
        ObjId obj_id;
        Zone* zone;
    public:
        virtual void execute () {};
        virtual const string message () = 0;
};

class WalkInvocation : public AbilityInvocation {
    private:
        list <Coord>* path;
    public:
        WalkInvocation (ObjId new_obj_id, Zone* new_zone,
                list<Coord>* new_path);
        ~WalkInvocation ();
        void execute ();
        const string message ();
};

class StepInvocation : public AbilityInvocation {
    private:
        int direction;
    public:
        StepInvocation (ObjId new_obj_id, Zone* new_zone, int new_direction);
        ~StepInvocation ();
        void execute ();
        const string message ();
};

class NullInvocation : public AbilityInvocation {
    public:
        NullInvocation (ObjId new_obj_id, Zone* zone);
        ~NullInvocation ();
        void execute ();
        const string message ();
};

class AttackInvocation : public AbilityInvocation {
    private:
        ObjId target_obj_id;
    public:
        AttackInvocation (ObjId new_obj_id, Zone* new_zone,
                ObjId new_target_obj_id);
        ~AttackInvocation ();
        void execute ();
        const string message ();
};

#endif
