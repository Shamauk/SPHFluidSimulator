#pragma once

#include "boundary.hpp"

class LinearBoundary : public Boundary {
public:
    LinearBoundary(float slope, float yIntercept, bool outOfBoundsAbove, float xLowRange, float xHighRange)
        : slope(slope),
        yIntercept(yIntercept),
        outOfBoundsAbove(outOfBoundsAbove),
        xLowRange(xLowRange),
        xHighRange(xHighRange)  {}

    void enforceBoundary(Particle &particle, float epsilon);

private:
    float slope;
    float yIntercept;
    bool outOfBoundsAbove;
    float xLowRange;
    float xHighRange;
};