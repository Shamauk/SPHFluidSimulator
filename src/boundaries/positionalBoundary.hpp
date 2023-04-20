#pragma once

#include "boundary.hpp"

class PositionalBoundary : public Boundary {
public:
    void enforceBoundary(Particle &particle, float epsilon) override;

    float setMinX(float x) { minX = x; }
    float setMaxX(float x) { maxX = x; }
    float setMinY(float y) { minY = y; }
    float setMaxY(float y) { maxY = y; }

private:
    const float BOUNDARY_DAMPING = -0.5;
    float minX;
    float maxX;
    float minY;
    float maxY;
};