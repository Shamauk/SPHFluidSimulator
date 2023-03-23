#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H

#include <vector>

#include "glm/glm.hpp"

#include "particle.h"
#include "kernel.h"

class FluidSimulator {
public:
    const glm::vec<3,double> ACCELERATION_DUE_TO_GRAVITY = glm::vec<3,double>(0.0,-9.8,0.0);
    const double SMOOTHING_LENGTH = 1.0;
    const double dt = 0.001;
    const double VISCOSITY = 1;
    
    std::vector<Particle::Particle> particles;

    void updateParticles();
    void addParticleFromXYZWithRestDensity(double x, double y, double z, double restDensity);
    std::vector<Particle::Particle>& getParticles() { return particles; }

private:
    // Boundary
    double boundaryDamping = 0.8;
    double minX = -5;
    double maxX = 5;
    double minY = -5;
    double maxY = 5;
    double minZ = -5;
    double maxZ = 5;

    glm::vec<3,double> computeForceFromGravity(Particle::Particle& particle);
    glm::vec<3,double> computeForceFromViscosity(Particle::Particle& particle);
    void findNeighbors(Particle::Particle& particle);
    glm::vec<3,double> computeForceFromPressure(Particle::Particle& particle);
    void findAndSetNeighborsForAllParticles();
    void computePartialVelocities();
    void computePartialDensity(Particle::Particle& particle);
    void computeDensity(Particle::Particle& particle);
    void computePartialDensities();
    void computePressuresFromPoisson();
    void computePressureForceOnParticle(Particle::Particle& particle);
    void computePressureForces();
    void updateFinalVelocityAndPosition();
    void handleBoundaryCollision(Particle::Particle& particle);
};
#endif