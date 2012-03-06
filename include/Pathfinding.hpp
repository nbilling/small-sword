#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "list"
#include "Types.hpp"
#include "Zone.hpp"
#include "MinHeap.hpp"
#include "Formulas.hpp"
#include "iostream"
#include "climits"

typedef struct {
  int** d;
  Coord** p;
} PathMap;

int pathfind_step_dijkstra (const Coord& src, const Coord& dest, Zone* zone);
PathMap dijkstra (const Coord& src, Zone* zone);
list<Coord>* find_path (const Coord& src, const Coord& dest, const PathMap& path_map);

#endif
