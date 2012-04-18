#include "Terminal.hpp"

Terminal::Terminal (int new_width) {
    before = new list<char*> ();
    after = new list<char*> ();
    width = new_width;
}

Terminal::~Terminal () {
    delete (before);
    delete (after);
}

void Terminal::scroll_up () {
    if (before->size () > 1) {
        char* old_cursor = before->front ();
        before->pop_front ();
        after->push_back (old_cursor);
    }
}

void Terminal::scroll_down () {
    if (after->size () > 0) {
        char* new_cursor = after->back ();
        after->pop_back ();
        before->push_front (new_cursor);
    }
}

void Terminal::scroll_top () {
    while (before->size () > 1) scroll_up ();
}

void Terminal::scroll_bottom () {
    while (after->size () > 0) scroll_down ();
}

void Terminal::append (const char* s) {
    // For each `\n' delimited substring in s, break it down again into
    // strings of, at most, length `width'. Then append these strings one at a
    // time to the terminal.
    char* s_copy = new char[strlen (s) + 1];
    strcpy (s_copy, s);
    for (char* t = strtok (s_copy, "\n"); t; t = strtok (NULL, "\n")) {
        int len = strlen (t);
        for (int i = 0; len / width > i; i++) {
            char* l = new char[width + 1];
            after->push_back (l);
            scroll_down ();
            memcpy (l, t + (i * width), width);
            l[width] = '\0';
        }
        if (len % width != 0) {
            char* l = new char[width + 1];
            after->push_back (l);
            scroll_down ();
            memcpy (l, t + (len - (len % width)), len % width);
            l[len % width] = '\0';
        }
    }
}

list<char*>* Terminal::get_lines (int n) {
    list<char*>* retval = new list<char*> ();
    list<char*>::iterator it = before->begin ();
    for (int i = 0; i < n && it != before->end (); i++) {
        retval->push_front (*it);
        it++;
    }
    return (retval);
}
