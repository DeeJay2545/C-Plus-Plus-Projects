// David J. Morvay
// Joey Pezzano
// Project 3
// disjoint.cc

#include "disjointset.h"
#include <numeric>
#include <iostream>

using namespace std;


DisjointSet::DisjointSet(uint32_t n) {
    // Create elements and rank array
    // Each with n elements
    ptr_elements = new uint32_t[n];
    ptr_rank = new uint32_t[n];
    
    nItems = n;
    
    // Initilaize elements[i]=i and rank[i]=0 for all slots
    for (int i=0; i<=n; i++) {
        ptr_elements[i] = i;
        ptr_rank[i] = 0;
    }
    
}

DisjointSet::~DisjointSet(void) {
    
    delete[] ptr_rank;
    delete[] ptr_elements;
}

uint32_t DisjointSet::find(uint32_t a) {
    // Algorithm 3 - Disjoint set find
    
    // Connect a directly to top of intree
    if (*(ptr_elements+a) != a) {
        *(ptr_elements+a) = find(*(ptr_elements+a));
    }
    
    return *(ptr_elements+a);  // Return top of intree
}

void DisjointSet::join(uint32_t a, uint32_t b) {
    // Algorithm 2 - Disjoint set union
    
    a = find(a);        // Get representatives for a and b
    b = find(b);
    
    if (a != b) {       // Only union if a and b are in different sets
        if (*(ptr_rank+a) < *(ptr_rank+b)) {        // Set with lower ranked merged into set with larger rank
            *(ptr_elements+a) = b;
        }
        else {
            if (*(ptr_rank+a) == *(ptr_rank+b)) {       // In case of tie, increment one set's rank
                *(ptr_rank+a) = *(ptr_rank+a) + 1;
            }
            *(ptr_elements+b) = a;
        }
    }
}
