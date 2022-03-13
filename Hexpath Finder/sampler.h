// David J. Morvay
// Joey Pezzano
// Project 3
// sampler.h

#include <cstdint>
#include <random>

#ifndef SAMPLER_H
#define SAMPLER_H

class Sampler {
    public:
        explicit Sampler(uint32_t n);
        ~Sampler();
    
        uint32_t getSample();
    
    private:
        uint32_t
            *items,
            nItems;
        std::random_device
            *rd;
        std::mt19937
            *mt;
};

#endif

