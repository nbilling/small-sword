#ifndef TERMINAL_HPP
#define TERMINAL_HPP

#include <list>
#include "string.h"

using namespace std;

class Terminal {
    private:
        // Note: the `cursor' position is the head of before.
        list<char*>* before;
        list<char*>* after;
        int width;

    public:
        Terminal (int new_width);

        ~Terminal ();

        void scroll_up ();

        void scroll_down ();

        void scroll_top ();

        void scroll_bottom ();

        void append (const char* s);

        list<char*>* get_lines (int n);
};

#endif
