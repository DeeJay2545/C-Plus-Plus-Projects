// David J. Morvay
// Joey Pezzano
// Project 3
// disjointset.h

#include <cstdint>
#include <stdexcept>

#ifndef DISJOINTSET_H
#define DISJOINTSET_H

class DisjointSet {
    public:
        explicit DisjointSet(uint32_t n);
        ~DisjointSet();
    
        uint32_t find(uint32_t a);
        void join(uint32_t a, uint32_t b);
    private:
        uint32_t
            nItems,
            *ptr_elements, *ptr_rank;
};

#endif

