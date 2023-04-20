#pragma once

#include "boundary.hpp"

class MultiBoundary : public Boundary {
public:
    void enforceBoundary(Particle &particle, float epsilon) {
        for (auto *boundary : boundaries) {
            boundary->enforceBoundary(particle, epsilon);
        }
    }

    void addBoundary(Boundary *boundary) {
        boundaries.push_back(boundary);
    }
    
private:
    std::vector<Boundary *> boundaries;
};