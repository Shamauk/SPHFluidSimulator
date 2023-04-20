#pragma once

#include "boundary.hpp"

class VerticalBoundary : public Boundary {
public:
    VerticalBoundary(float X, float yLowRange, float yHighRange, bool outOfBoundsOnRight)
        : X(X),
        yLowRange(yLowRange),
        yHighRange(yHighRange),
        outOfBoundsOnRight(outOfBoundsOnRight) {}

    void enforceBoundary(Particle &particle, float epsilon);

private:
    float X;
    float yLowRange;
    float yHighRange;
    bool outOfBoundsOnRight;
    float DAMPING = -0.5;
};