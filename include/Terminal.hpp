#ifndef TERMINAL_HPP
#define TERMINAL_HPP

class Terminal;

#include <list>
#include <string>

#include "Formulas.hpp"

using namespace std;

class Terminal {
    private:
        // Note: the `cursor' position is the head of before.
        list<string>* before;
        list<string>* after;
        int width;

    public:
        Terminal (int new_width);

        ~Terminal ();

        void scroll_up ();

        void scroll_down ();

        void scroll_top ();

        void scroll_bottom ();

        void append (const string s);

        list<string>* get_lines (int n);
};

#endif
