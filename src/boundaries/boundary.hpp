#pragma once

#include <iostream>

#include "../datastructures/particle.hpp"

class Boundary {
public:
    virtual void enforceBoundary(Particle &particle, float epsilon) = 0;
};