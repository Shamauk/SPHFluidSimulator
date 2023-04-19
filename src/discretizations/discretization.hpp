#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "../datastructures/particle.hpp"
#include "../utils/constVector.hpp"

class Discretization {
public:
    Discretization(float viewWidth, float viewHeight, float kernelRange, std::string name) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->kernelRange = kernelRange;
        this->name = name;
    }

    virtual void getNeighbors(ConstVectorWrapper<Particle>) = 0;
    virtual size_t getMemoryUse() = 0;

    std::string getDiscretizationName() {
        return name;
    }

protected:
    float viewWidth;
    float viewHeight;
    float kernelRange;

    std::string name;
};