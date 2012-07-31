#ifndef MIN_HEAP_HPP
#define MIN_HEAP_HPP

#include <map>
#include <iostream>

using namespace std;

// Please note that the class T must have:
// < and - (less than and minus).
// As well has a function:
// int hash (T)
// , which will return a unique integer for
// all T (a perfect hash function).

template <class T>
class MinHeap {
    private:
        T* data;
        map<int,int> hash_map;
        int capacity;
        int size;

        static inline int left_child_of (int x) {
            return (x * 2 + 1);
        }

        static inline int right_child_of (int x) {
            return (x * 2 + 2);
        }

        static inline int parent_of (int x) {
            return ((x - 1) / 2);
        }

        inline void switch_hashes (T x, T y) {
            int temp = hash_map[hash (x)];
            hash_map[hash (x)] = hash_map[hash (y)];
            hash_map[hash (y)] = temp;
        }

        inline void switch_elems (int x, int y) {
            T& temp = data[x].copy();
            data[x] = data[y];
            data[y] = temp;
        }

        void shift_up (int x) {
            //Shifts element at x upwards, as necessary,
            //returns where x ends up.
            int cur = x;

            //Loop can't get stuck at root since "(0 - 1) / 2 = 0"
            //and "data[0] < data[0]" is trivially false.
            while (data[cur] < data[parent_of (cur)]) {
                switch_hashes (data[cur], data[parent_of (cur)]);
                switch_elems (cur, parent_of (cur));
                cur = parent_of (cur);
            }
        }

        void shift_down (int x) {
            //Shifts element at x downwards, as necessary,
            //returns where x ends up.
            //If x is empty do nothing
            if (x >= size)
                return;

            int cur = x;

            //Loop while cur has at least one child
            while (left_child_of (cur) < size){
                int max_child = left_child_of (cur);
                if (right_child_of (cur) < size)
                    if (data[right_child_of (cur)] < data[left_child_of (cur)])
                        max_child = right_child_of (cur);
                if (data[max_child] < data[cur]) {
                    switch_elems (cur, max_child);
                    switch_hashes (data[cur], data[max_child]);
                    cur = max_child;
                }
                else
                    //Break early if no more swaps necessary.
                    break;
            }
        }

    public:

        MinHeap (int new_capacity) {
            data = new T[new_capacity];
            capacity = new_capacity;
            size = 0;
        }

        ~MinHeap (){
            delete data;
        }

        void push (const T& new_object) {
            // If full then do nothing
            if (size >= capacity)
                return;

            size++;
            data[size - 1] = new_object;
            hash_map[hash (new_object)] = size - 1;
            shift_up (size - 1);
        }

        T& pop () {
            T& top = data[0].copy();
            T bottom = data[size - 1];

            //Move bottom to top, reduce size.
            data[0] = data[size - 1];
            size--;

            hash_map[hash (bottom)] = 0;
            shift_down (0);

            return top;
        }

        // void decrease_key (T& elem, int new_key) {
        //   int elem_heap_location = hash_map[hash (elem)];
        //   data[elem_heap_location] = set_key (elem, new_key); //data[hash_map[hash (elem)]] - d;
        //   shift_up (elem_heap_location);
        // }
        void decrease_key (int hash_key, int new_key) {
            int elem_heap_location = hash_map[hash_key];
            T& elem = data[elem_heap_location];
            data[elem_heap_location] = set_key (elem, new_key);
            shift_up (elem_heap_location);
        }

        bool is_empty () {
            return (size == 0);
        }

};

#endif
