#pragma once

#include "boundary.hpp"

class PositionalBoundary : public Boundary {
public:
    void enforceBoundary(Particle &particle, float epsilon) override;

private:
    const float BOUNDARY_DAMPING = -0.5;
};