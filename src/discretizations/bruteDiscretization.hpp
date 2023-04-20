#pragma once

#include "discretization.hpp"

class BruteDiscretization : public Discretization {
public:
    BruteDiscretization(float viewWidth, float viewHeight, float kernelRange) 
        : Discretization(viewWidth, viewHeight, kernelRange, "Brute") {}

    void getNeighbors(ConstVectorWrapper<Particle>) override;

    size_t getMemoryUse() override {
        return 0;
    }
};