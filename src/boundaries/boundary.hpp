#pragma once

#include <iostream>

#include "../datastructures/particle.hpp"

class Boundary {
public:
    virtual void enforceBoundary(Particle &particle, float epsilon) = 0;

    void resetBoundary(float viewWidth, float viewHeight) {
        this->minX = 0.f;
        this->minY = 0.f;
        this->maxX = viewWidth;
        this->maxY = viewHeight;
    }

    void setMinX(float minX) { this->minX = minX; }
    void setMinY(float minY) { this->minY = minY; }
    void setMaxX(float maxX) { this->maxX = maxX; }
    void setMaxY(float maxY) { this->maxY = maxY; }

    float getMinX() { return this->minX; }
    float getMinY() { return this->minY; }
    float getMaxX() { return this->maxX; }
    float getMaxY() { return this->maxY; }
protected:
    float minX;
    float minY;
    float maxX;
    float maxY;
};