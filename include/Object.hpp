#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "cstring"
#include "cstdlib"
#include "list"
#include "iostream"
#include "libtcod.hpp"
#include "Types.hpp"
#include "Zone.hpp"
#include "CSheet.hpp"

using namespace std;

class Object {
private:
  static int next_id;
  static list<int>* recycled_ids;
  static int object_count;

  int get_next_id ();
  void recycle_id ();

public:
  int id;
  char c;
  char* name;
  TCODColor color;
  bool blocks;
  CSheet* csheet;

  Object (char new_c, const char* new_name, TCODColor new_color, 
          bool new_blocks, CSheet* new_csheet);

  ~Object ();
};

#endif
