#ifndef FLUID_H
#define FLUID_H

#include <vector>

#include "particle.h"
#include "kernel.h"

class Fluid {
public:
    Fluid() {}

    void createScene(short sceneNumber);
    void updateParticleStates();
    
    const std::vector<Particle>& getParticles() const {
        return particles;
    }

    const glm::vec3& getParticleColor() const {
        return particleColor;
    }

    double getParticleRadius() {
        return particleRadius;
    }

    double getMinX() { return minX; }
    double getMaxX() { return maxX; }
    double getMinY() { return minY; }
    double getMaxY() { return maxY; }

    double setMinY(double y) { minY = y; }
    double setMaxY(double y) { maxY = y; }

private:
    double particleRadius = 0.01;
    glm::vec3 particleColor = glm::vec3(0.0f, 0.1f, 1.0f); 
    const double SMOOTHING_LENGTH = particleRadius * 4;
    Kernel kernel = Kernel(SMOOTHING_LENGTH);

    // Boundary
    double minX = -1;
    double maxX = 1;
    double minY = -1;
    double maxY = 1;

    std::vector<Particle> particles;
    void handleBoundaryConditions();
    void calculateDensity();
};

#endif 