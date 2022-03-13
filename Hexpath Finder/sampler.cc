// David J. Morvay
// Joey Pezzano
// Project 3
// sampler.cc

#include "sampler.h"
#include <numeric>
#include <iostream>

Sampler::Sampler(uint32_t n) {
    // Create an array of n integers
    items = new uint32_t[n];
    
    // Store n as class variable
    nItems = n;
    
    // Set array[i] = i for all slots
    for (int i=0; i<n; i++) {
        items[i] = i;
    }
    
    rd = new std::random_device;
    mt = new std::mt19937((*rd)());
}

Sampler::~Sampler(void) {
    
    delete[] items;
}

uint32_t Sampler::getSample()
{
    uint32_t
        r,
        tmp;
    std::uniform_int_distribution<>
        dis(0,nItems-1);
    
    // step 1: select random position
    r = (uint32_t)dis(*mt);
    
    // step 2: remember element at that position
    tmp = items[r];
    
    // step 3: decrement n
    nItems--;
    
    // step 4: move last element in list into the hole left by selection
    items[r] = items[nItems];
    
    // step 5: return selected element
    return tmp;
}
