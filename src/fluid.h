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
        return PARTICLE_RADIUS;
    }

    double getMinX() { return minX; }
    double getMaxX() { return maxX; }
    double getMinY() { return minY; }
    double getMaxY() { return maxY; }

    void setMinY(double y) { minY = y; }
    void setMaxY(double y) { maxY = y; }

    double getSmoothingLength() { return SMOOTHING_LENGTH; }

private:
    const double PARTICLE_RADIUS = 0.01;
    const double PARTICLE_MASS = 0.00419;
    const double VISCOSITY = 0.1;
    const double PRESSURE_STIFFNESS = 100.0;
    const double REST_DENSITY = 1000.0;
    const double dt = 0.00005;
    const double BOUNDARY_DAMPING = -0.4;
    glm::vec3 particleColor = glm::vec3(0.0f, 0.1f, 1.0f); 
    const double SMOOTHING_LENGTH = PARTICLE_RADIUS * 4;
    Kernel kernel = Kernel(SMOOTHING_LENGTH);

    // Boundary
    double minX = -1;
    double maxX = 1;
    double minY = -1;
    double maxY = 1;

    // Scenes
    const int DAM_PARTICLES = 100;

    std::vector<Particle> particles;
    void handleBoundaryConditions();
    void calculateDensity();
    void calculateIntermediateVelocity();
    void calculatePressureAndPressureForce();
    void calculatePressure(Particle &p);
    void updateParticleVelocityAndPosition();
};

#endif 