#pragma once

#include "boundary.hpp"

class PositionalBoundary : public Boundary {
public:
    void enforceBoundary(Particle &particle, float epsilon) override;

    void setMinX(float x) { minX = x; }
    void setMaxX(float x) { maxX = x; }
    void setMinY(float y) { minY = y; }
    void setMaxY(float y) { maxY = y; }

private:
    const float BOUNDARY_DAMPING = -0.5;
    float minX;
    float maxX;
    float minY;
    float maxY;
};