#include "Terminal.hpp"

Terminal::Terminal (int new_width) {
    before = new list<string> ();
    after = new list<string> ();
    width = new_width;
}

Terminal::~Terminal () {
    delete (before);
    delete (after);
}

void Terminal::scroll_up () {
    if (before->size () > 1) {
        string old_cursor = before->front ();
        before->pop_front ();
        after->push_back (old_cursor);
    }
}

void Terminal::scroll_down () {
    if (after->size () > 0) {
        string new_cursor = after->back ();
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

// Returns the length of the susbtring starting at index 0 and ending at the
// first Soft End Of Line. Note this treats '\n' as a regular whitespace char
// and treats '-' as a whitespace char.
int soft_wrap_length (int n, string s) {
    if ((int) s.length () <= n)
        return (s.length ());
    else {
        int length = n;
        int i = 1;
        for (string::iterator it = s.begin ();
                it != s.end () && i <= n; it++) {
            if (*it == ' ' || *it == '\n' || *it == '\t' || *it == '-') {
                length = i;
            }
            i++;
        }
        return length;
    }
}

string substring (const string s, int a, int b) {
    if (a < (int) s.length ()) return (s.substr (a, b));
    else return ("");
}

void Terminal::append (const string s) {
    string rest = s;
    while (rest.compare ("") != 0) {
        int l = soft_wrap_length (width, rest);
        after->push_back (substring (rest, 0, l));
        scroll_down ();
        rest = substring (rest, l, string::npos);
    }
}

list<string>* Terminal::get_lines (int n) {
    list<string>* retval = new list<string> ();
    list<string>::iterator it = before->begin ();
    for (int i = 0; i < n && it != before->end (); i++) {
        retval->push_front (*it);
        it++;
    }
    return (retval);
}
