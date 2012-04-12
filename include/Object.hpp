#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "cstring"
#include "cstdlib"
#include "list"
#include "iostream"
#include "libtcod.hpp"
#include "Types.hpp"
#include "Zone.hpp"

using namespace std;

class Object {
    protected:
        int id;
        char c;
        char* name;
        TCODColor color;
        bool blocks;

        static int next_id;
        static list<int>* recycled_ids;
        static int object_count;
        static map<int, Object*>* object_registry;

        int get_next_id ();
        void recycle_id ();

    public:
        virtual ~Object () {};

        static void delete_registered_objects ();

        static Object* get_object_by_id (int target_id);

        int get_id ();

        char get_char ();

        char* get_name ();

        TCODColor get_color ();

        bool get_blocks ();
};

class Lifeform : public Object {
    protected:
        int max_hp;
        int hp;
        struct {
            int back;
            int right_hand;
        } equipment;

    public:
        Lifeform (char new_c, const char* new_name, TCODColor new_color,
                bool new_blocks, int new_max_hp);

        ~Lifeform ();

        int get_max_hp ();

        int get_hp ();

        int get_equipped_back ();

        int get_equipped_right_hand ();

        void set_max_hp (int new_max_hp);

        void set_hp (int new_hp);

        void equip_back (int obj_id);

        void equip_right_hand (int obj_id);
};

class Weapon : public Object {
    protected:
        bool swingable;
        int swing_damage;
        int swing_range;

    public:
        Weapon (char new_c, const char* new_name, TCODColor new_color,
                bool new_blocks, bool new_swingable, int new_swing_damage,
                int new_swing_range);

        ~Weapon ();

        bool get_swingable ();

        int get_swing_damage ();

        int get_swing_range ();
};

#endif
