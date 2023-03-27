#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H

#include <vector>

#include "glm/glm.hpp"

#include "particle.h"
#include "kernel.h"

class FluidSimulator {
public:
    const glm::dvec3 ACCELERATION_DUE_TO_GRAVITY = glm::dvec3(0.0,-9.8,0.0);
    const double PARTICLE_RADIUS = 0.075;
    const double SMOOTHING_LENGTH = 4 * PARTICLE_RADIUS;
    const double dt = 0.0005;
    const double VISCOSITY = 0.01;
    const double REST_DENSITY = 1000; 
    const double THRESHOLD = 0.0001;
    const double RELAXATION_FACTOR = 0.5;
    const double STIFFNESS = 0.01;

    // Boundary
    const float BOUNDARY_RESTITUTION = 0.5;
    const int minX = -1;
    const int maxX = 1;
    const int minY = -1;
    const int maxY = 1;
    double minZ = -1;
    const int maxZ = 1;
    
    std::vector<Particle::Particle> particles;
    std::vector<Particle::Particle> boundaries;

    int numXGrids;
    int numYGrids;
    int numZGrids;
    std::vector<Particle::Particle*>*** grid;

    void updateParticles();
    void addParticleFromXYZ(double x, double y, double z);
    void addBoundaryFromXYZ(double x, double y, double z);
    void addParticle(Particle::Particle& particle);
    std::vector<Particle::Particle>& getParticles() { return particles; }
    FluidSimulator();

private:
    Kernel kernel = Kernel(SMOOTHING_LENGTH);

    bool isIndexOutOfBoundsOfGrid(int x_index, int y_index, int z_index);
    void findNeighbors(Particle::Particle& particle);
    void assignParticlesToGrid();
    void findAndSetNeighborsForAllParticles();
    void handleBoundaryCollision(Particle::Particle& particle);
    void computeDensityPressureForAllParticles();
    void computeForcesForAllParticles();
    void integrateAllParticles();
    void enforceBoundaryConditionOnAllParticles();
    void computeDensityForAllParticles();
    void setIntermediateVelocityForAllParticles();
    void setAllPressuresAndSetPressureForce();
    void clearGrid();
};
#endif