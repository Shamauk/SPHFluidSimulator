#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H

#include <vector>

#include "glm/glm.hpp"

#include "particle.h"
#include "kernel.h"

class FluidSimulator {
public:
    glm::vec<3,double> ACCELERATION_DUE_TO_GRAVITY = glm::vec<3,double>(0.0,-9.8,0.0);
    double SMOOTHING_LENGTH = 1.0;
    double dt = 0.001;
    double viscosity;

    // Implicit solve
    double MAX_ITERATIONS = 100;
    double TOLERANCE = 0.0001;

    
    std::vector<Particle::Particle> particles;

    void updateParticles();
    void addParticleFromXYZWithRestDensity(double x, double y, double z, double restDensity);
    std::vector<Particle::Particle>& getParticles() { return particles; }

private:
    glm::vec<3,double> computeForceFromGravity(Particle::Particle& particle);
    glm::vec<3,double> computeForceFromViscosity(Particle::Particle& particle);
    std::vector<Particle::Particle*> findNeighbors(Particle::Particle& particle);
    glm::vec<3,double> computeForceFromPressure(Particle::Particle& particle);
    void getNeighbors();
    void computePartialVelocities();
    void computePartialDensity(Particle::Particle& particle);
    void computeDensity(Particle::Particle& particle);
    void computePartialDensities();
    void computePressuresFromPoisson();
    void computePressureForceOnParticle(Particle::Particle& particle);
    void computePressureForces();
    void updateFinalVelocityAndPosition();
};
#endif