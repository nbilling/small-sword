#ifndef OBJECT_HPP
#define OBJECT_HPP

class ObjId;
class Object;
class Lifeform;
class Weapon;

#include "assert.h"
#include "cstdlib"
#include "iostream"
#include "list"

#include "libtcod.hpp"

#include "Types.hpp"
#include "Zone.hpp"

using namespace std;

class ObjId {
    private:
        int id;
    public:
        ObjId (int new_id=0);

        ObjId& operator++ ();

        ObjId& operator= (const ObjId& rhs);

        ObjId& operator= (int new_id);

        bool operator< (const ObjId& rhs) const;

        bool operator! ();

        operator bool ();
};

// Note: id=0 is the null object, there should never be an object with this id.
class Object {
    protected:
        ObjId id;
        char c;
        string name;
        TCODColor color;
        bool blocks;

        static ObjId next_id;
        static list<ObjId>* recycled_ids;
        static int object_count;
        static map<ObjId, Object*>* object_registry;

        ObjId get_next_id ();
        void recycle_id ();

    public:
        virtual ~Object () {};

        static void delete_registered_objects ();

        static Object* get_object_by_id (ObjId target_id);

        virtual ObjType type () = 0;

        ObjId get_id ();

        char get_char ();

        const string& get_name ();

        TCODColor get_color ();

        bool get_blocks ();

        void set_char (char new_c);

        void set_color (TCODColor new_color);

        void set_blocks (bool new_blocks);
};

class Lifeform : public Object {
    protected:
        int max_hp;
        int hp;
        struct {
            ObjId back;
            ObjId right_hand;
        } equipment;

    public:
        Lifeform (char new_c, const string& new_name, TCODColor new_color,
                bool new_blocks, int new_max_hp);

        ~Lifeform ();

        ObjType type ();

        int get_max_hp ();

        int get_hp ();

        ObjId get_equipped_back ();

        ObjId get_equipped_right_hand ();

        void set_max_hp (int new_max_hp);

        void set_hp (int new_hp);

        void equip_back (ObjId obj_id);

        void equip_right_hand (ObjId obj_id);
};

class Weapon : public Object {
    protected:
        bool swingable;
        int swing_damage;
        int swing_range;

    public:
        Weapon (char new_c, const string& new_name, TCODColor new_color,
                bool new_blocks, bool new_swingable, int new_swing_damage,
                int new_swing_range);

        ~Weapon ();

        ObjType type ();

        bool get_swingable ();

        int get_swing_damage ();

        int get_swing_range ();
};
#endif
