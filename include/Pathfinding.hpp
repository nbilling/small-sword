#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "climits"
#include "iostream"
#include "list"

#include "Formulas.hpp"
#include "MinHeap.hpp"
#include "Types.hpp"
#include "Zone.hpp"

#define INF INT_MAX

using namespace std;

typedef struct {
    int** d;
    Coord** p;
} PathMap;

int pathfind_step_dijkstra (const Coord& src, const Coord& dest, Zone* zone);
PathMap dijkstra (const Coord& src, Zone* zone);
list<Coord>* find_path (const Coord& src, const Coord& dest, const PathMap& path_map);

#endif
