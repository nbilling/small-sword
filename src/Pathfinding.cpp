#include "Pathfinding.hpp"
#include "iostream"

#define INF INT_MAX

using namespace std;

class Node {
public:
  Coord c;
  int dist;

  Node (Coord new_c, int new_dist) {
    c = new_c;
    dist = new_dist;
  }

  Node () {
    c = (Coord){0,0};
    dist = 0;
  }

  bool operator< (const Node& rhs) {
    // Less than compares distance
    return (this->dist < rhs.dist);
  }

  Node& operator= (const Node& rhs) {
    // Assignment copies the name (x,y) and distance
    // of the dest into the src.
    if (this != (&rhs)) {
      this->c = rhs.c;
      this->dist = rhs.dist;
    }
    return *this;
  }

  Node& copy () {
    return (* new Node (this->c, this->dist));
  }
};

inline int hash (const Node& n) {
  // This hash function is a perfect hash assuming the 
  // grid these coordinates are on is at most 180x180.
  return (n.c.x + (180 * n.c.y));
}

inline Node& set_key (Node& n, int new_dist) {
  n.dist = new_dist;
  return n;
}

inline int hash_coords (Coord c) {
  return (c.x + 180 * c.y);
}

inline void consider_edges (const Coord& src, Zone* zone, int** d, Coord** p, MinHeap<Node>* u) {
  // For each of the 8 tiles surrounding (x,y), if the distance to
  // that tile in d is greater than d[x][y] + 1, then update d at that
  // tile to d[x][y] + 1.
  int x = src.x;
  int y = src.y;

  if (zone->in_bounds ((Coord){x-1, y+1}))
    if ((!zone->is_blocked ((Coord){x-1, y+1})) 
        && (d[x][y] + 1 < d[x-1][y+1])) {
      u->decrease_key (hash_coords ((Coord){x-1, y+1}), d[x][y] + 1);
      d[x-1][y+1] = d[x][y] + 1;
      p[x-1][y+1] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x, y+1}))
    if ((!zone->is_blocked ((Coord){x, y+1})) 
        && (d[x][y] + 1 < d[x][y+1])) {
      u->decrease_key (hash_coords ((Coord){x, y+1}), d[x][y] + 1);
      d[x][y+1] = d[x][y] + 1;
      p[x][y+1] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x+1, y+1}))
    if ((!zone->is_blocked ((Coord){x+1, y+1})) 
        && (d[x][y] + 1 < d[x+1][y+1])) {
      u->decrease_key (hash_coords ((Coord){x+1, y+1}), d[x][y] + 1);
      d[x+1][y+1] = d[x][y] + 1;
      p[x+1][y+1] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x-1, y}))
    if ((!zone->is_blocked ((Coord){x-1, y})) 
        && (d[x][y] + 1 < d[x-1][y])) {
      u->decrease_key (hash_coords ((Coord){x-1, y}), d[x][y] + 1);
      d[x-1][y] = d[x][y] + 1;
      p[x-1][y] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x+1, y}))
    if ((!zone->is_blocked ((Coord){x+1, y})) 
        && (d[x][y] + 1 < d[x+1][y])) {
      u->decrease_key (hash_coords ((Coord){x+1, y}), d[x][y] + 1);
      d[x+1][y] = d[x][y] + 1;
      p[x+1][y] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x-1, y-1}))
    if ((!zone->is_blocked ((Coord){x-1, y-1})) 
        && (d[x][y] + 1 < d[x-1][y-1])) {
      u->decrease_key (hash_coords ((Coord){x-1, y-1}), d[x][y] + 1);
      d[x-1][y-1] = d[x][y] + 1;
      p[x-1][y-1] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x, y-1}))
    if ((!zone->is_blocked ((Coord){x, y-1})) 
        && (d[x][y] + 1 < d[x][y-1])) {
      u->decrease_key (hash_coords ((Coord){x, y-1}), d[x][y] + 1);
      d[x][y-1] = d[x][y] + 1;
      p[x][y-1] = (Coord) {x,y};
    }
  if (zone->in_bounds ((Coord){x+1, y-1}))
    if ((!zone->is_blocked ((Coord){x+1, y-1})) 
        && (d[x][y] + 1 < d[x+1][y-1])) {
      u->decrease_key (hash_coords ((Coord){x+1, y-1}), d[x][y] + 1);
      d[x+1][y-1] = d[x][y] + 1;
      p[x+1][y-1] = (Coord) {x,y};
    }
}

int pathfind_step_dijkstra (const Coord& src, const Coord& dest, Zone* zone) {

  int src_x = src.x;
  int src_y = src.y;
  int dest_x = dest.x;
  int dest_y = dest.y;

  //If destination is blocked then will always say to stand still
  //Don't call this function on a tile that could be blocked
  //If the destination is unblocked but unreachable then this function WILL 
  //attempt to pop an empty heap.
  if (zone->is_blocked ((Coord){dest_x, dest_y}))
    return 5;

  // Dijkstra's algorithm for pathfinding in small-sword

  // Initialise weight heap
  MinHeap<Node>* u = new MinHeap<Node> (zone->grid_w * zone->grid_h);
  for (int i=0; i < zone->grid_w; i ++)
    for (int j=0; j < zone->grid_h; j++)
      u->push (* new Node ((Coord){i,j}, INF));
  
  // Initialise dist array
  int** d = new int*[zone->grid_w];
  for (int i=0; i < zone->grid_w; i ++){
    d[i] = new int[zone->grid_h];
    for (int j=0; j < zone->grid_h; j++)
      d[i][j] = INF;
  }

  // Initialise prev array
  Coord** p = new Coord*[zone->grid_w];
  for (int i=0; i < zone->grid_w; i++)
    p[i] = new Coord[zone->grid_h];
               
  // Set current xy
  int cur_x = src_x;
  int cur_y = src_y;

  // Set current xy tentative dist to 0
  u->decrease_key (hash_coords ((Coord) {cur_x, cur_y}), 0);
  d[cur_x][cur_y] = 0;

  // While current xy != target
  while (!(cur_x == dest_x && cur_y == dest_y)) {
    if (u->is_empty()) {
      cerr << "Pathfinding => pathfind_step_dijkstra =>"
        "Heap empty before path found."
           << endl;
      return 5;
    }
         
    // Consider current node's edges
    consider_edges ((Coord){cur_x, cur_y}, zone, d, p, u);

    // Pick least node from heap and remove it
    Node cur_n = u->pop();

    // If distance to cur_n is infinite then there's
    // no path to dest.
    if (cur_n.dist == INF) {
      cerr << "Pathfinding => pathfind_step_dijkstra =>"
        "Sought node unreachable." << endl;
      return 5;
    }

    // Set current xy to node's xy
    cur_x = cur_n.c.x;
    cur_y = cur_n.c.y;
  }

  while (!(p[cur_x][cur_y].x == src_x && p[cur_x][cur_y].y == src_y)) {
    if (p[cur_x][cur_y].x == cur_x && p[cur_x][cur_y].y == cur_y) {
      cerr << "Pathfinding => pathfind_step_dijkstra =>" 
        "Very weird infinite loop encountered" << endl;
      return 5;
    }
    int temp = cur_x;
    cur_x = p[cur_x][cur_y].x;
    cur_y = p[temp][cur_y].y;
  }

  delete u;
  for (int i=0; i < zone->grid_w; i ++)
    delete d[i];
  delete d;
  for (int i=0; i < zone->grid_w; i++)
    delete p[i];
  delete p;

  cout << displacement_to_direction (cur_x - src_x, cur_y - src_y) << endl;
  return (displacement_to_direction (cur_x - src_x, cur_y - src_y));
}
