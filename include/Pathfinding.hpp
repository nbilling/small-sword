#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "Types.hpp"
#include "Zone.hpp"
#include "MinHeap.hpp"
#include "Formulas.hpp"
#include "iostream"
#include "climits"

int pathfind_step_dijkstra (const Coord& src, const Coord& dest, Zone* zone);

#endif
