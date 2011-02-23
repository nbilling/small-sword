//Standard library
#include "cmath"
#include "list"
using namespace std;

//My files
#include "Tile.cpp"
#include "Object.cpp"

class Zone{
public:
  //List of objects in Zone
  list<Object*>* objects;
  //Two-dimensional array of Tiles
  Tile::Tile*** grid;
  //Grid dimensions
  int grid_w;
  int grid_h;

  Zone (int new_grid_w, int new_grid_h){
    /*Creates a new zone.
      - 'grid' filled with tiles using default tile constructor.
      - 'objects' list empty.*/
    grid = (Tile::Tile***) malloc (sizeof (Tile::Tile**) * new_grid_w);
    for (int x=0; x < new_grid_w; x++){
      grid[x] = (Tile**) malloc (sizeof (Tile::Tile*) * new_grid_h);
      for (int y=0; y < new_grid_h; y++){
        grid[x][y] = new Tile::Tile();
      }
    }
    grid_w = new_grid_w;
    grid_h = new_grid_h;
    objects = new list<Object*>();
  }

  ~Zone () {
    /*WARNING: This destructor will PURGE all memory pointed to by the Zone.
      If you want to keep some Objects then copy their pointers and remove
      them from 'objects' list in this zone FIRST.*/
    
    for (int x=0; x < grid_w; x++) {
      //Delete all tiles in this row (Tile::Tile*)
      for (int y=0; y < grid_h; y++){
        delete (grid[x][y]);
      }
      //Delete pointer to this row (Tile::Tile**)
      free (grid[x]);
    }
    //Delete base of row pointers (Tile::Tile***)
    free (grid);
    //Delete all objects in 'objects' list
    for (list<Object*>::iterator o = objects->begin(); o != objects->end(); o++){
      delete (*o);
    }
    //Delete objects list
    delete objects;
  }

  bool is_blocked (int x, int y) {
    if (grid[x][y]->blocked){
      return true;
    }
    for (list<Object*>::iterator o = objects->begin(); o != objects->end(); o++){
      if ((*o)->blocks && (*o)->x == x && (*o)->y == y){
        return (true);
      }     
    }
    return (false);
  }


  int distance_to (Object* src_object, Object* dest_object){
    float dx = (float) dest_object->x - src_object->x;
    float dy = (float) dest_object->y - src_object->y;    
    return ((int) sqrt (pow (dx, 2) + pow (dy, 2)));
  }
 
  void move_object (Object* object, int dx, int dy){
    if (!is_blocked (object->x + dx, object->y + dy)) {
      object->move (dx, dy);
    }
  }

  void move_object_towards (Object* object, int target_x, int target_y){
    float dx = (float) (target_x - object->x);
    float dy = (float) (target_y - object->y);
    float distance = sqrt (pow (dx, 2) + pow(dy, 2));
 
    //normalize it to length 1 (preserving direction), then round it and
    //convert to integer so the movement is restricted to the grid
    int move_x = round (dx / distance);
    int move_y = round (dy / distance);
    this->move_object(object, move_x, move_y);
  }
};
