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
        CSheet* csheet;

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

        int get_object_id ();

        char get_object_char ();

        char* get_object_name ();

        TCODColor get_object_color ();

        bool get_object_blocks ();
};

class Lifeform : public Object {
    private:
        int max_hp;
        int hp;

    public:
        Lifeform (char new_c, const char* new_name, TCODColor new_color,
                bool new_blocks, int new_max_hp);

        ~Lifeform ();

        int get_max_lifeform_hp ();

        int get_lifeform_hp ();

        void set_lifeform_max_hp (int new_max_hp);

        void set_lifeform_hp (int new_hp);
};

#endif
