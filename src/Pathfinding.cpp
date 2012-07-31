#include "Pathfinding.hpp"

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
            // Assignment copies (x,y) and distance of the dest into the src.
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

inline void consider_edges (const Coord& src, Zone* zone, int** d, Coord** p,
        MinHeap<Node>* u) {
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

PathMap dijkstra (const Coord& src, Zone* zone) {
    // Dijkstra's algorithm for pathfinding in small-sword

    MinHeap<Node>* u = new MinHeap<Node> (zone->width () * zone->height ());
    for (int i=0; i < zone->width (); i ++)
        for (int j=0; j < zone->height (); j++)
            u->push (* new Node ((Coord){i,j}, INF));

    int** d = new int*[zone->width ()];
    for (int i=0; i < zone->width (); i ++){
        d[i] = new int[zone->height ()];
        for (int j=0; j < zone->height (); j++)
            d[i][j] = INF;
    }

    Coord** p = new Coord*[zone->width ()];
    for (int i=0; i < zone->width (); i++)
        p[i] = new Coord[zone->height ()];

    Coord cur = src;

    u->decrease_key (hash_coords (cur), 0);
    d[cur.x][cur.y] = 0;

    // While there are more reachable, unvisited nodes left
    while (!u->is_empty () && d[cur.x][cur.y] != INF) {
        consider_edges (cur, zone, d, p, u);
        Node cur_n = u->pop();
        cur = cur_n.c;
    }

    delete u;
    return ((PathMap){d,p});
}

list<Coord>* find_path (const Coord& src, const Coord& dest,
        const PathMap& path_map) {
    list<Coord>* retval = new list<Coord> ();

    int** d = path_map.d;
    Coord** p = path_map.p;

    if (d[dest.x][dest.y] == INF) {
        // Sought node unreachable
        return retval;
    }

    Coord cur = dest;
    while (!coord_eq (cur, src)) {
        retval->push_front (cur);
        cur = p[cur.x][cur.y];
    }
    retval->push_front (cur);

    return (retval);
}

// Find the shortest path from src to dest and return it.
list<Coord>* pathfind_dijkstra (const Coord& src, const Coord& dest,
        Zone* zone) {

    PathMap path_map = dijkstra (src, zone);

    list<Coord>* retval = find_path (src, dest, path_map);

    for (int i=0; i < zone->width (); i ++)
        delete path_map.d[i];
    delete path_map.d;
    for (int i=0; i < zone->width (); i++)
        delete path_map.p[i];
    delete path_map.p;

    return (retval);
}

// Find the shortest path from src to dest, then return the first step
// of the path.
int pathfind_step_dijkstra (const Coord& src, const Coord& dest, Zone* zone) {
    list<Coord>* path = pathfind_dijkstra (src, dest, zone);
    if (path->size () > 1) {
        Coord step_start = path->front ();
        path->pop_front ();
        Coord step_end = path->front ();
        path->pop_front ();
        delete (path);
        return (displacement_to_direction (step_end.x - step_start.x,
                    step_end.y - step_start.y));
    }
    else return (5);
}

