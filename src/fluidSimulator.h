#ifndef FLUID_SIMULATOR_H
#define FLUID_SIMULATOR_H

#include <vector>

#include "glm/glm.hpp"

#include "particle.h"
#include "kernel.h"

class FluidSimulator {
public:
    const glm::dvec3 ACCELERATION_DUE_TO_GRAVITY = glm::dvec3(0.0,-9.8,0.0);
    const double SMOOTHING_LENGTH = 0.1;
    const double dt = 0.05;
    const double VISCOSITY = 0.0001; // m^2/s
    const double REST_DENSITY = 1000; // kg/m^3 (for water)
    const double THRESHOLD = 0.0001;
    const double RELAXATION_FACTOR = 0.5;
    const double STIFFNESS = 0.3;

    // Boundary
    const float BOUNDARY_RESTITUTION = 0.5;
    const int minX = -5;
    const int maxX = 5;
    const int minY = -5;
    const int maxY = 5;
    const int minZ = -5;
    const int maxZ = 5;
    
    std::vector<Particle::Particle> particles;

    int numXGrids;
    int numYGrids;
    int numZGrids;
    std::vector<Particle::Particle*>*** grid;

    void updateParticles();
    void addParticleFromXYZ(double x, double y, double z);
    std::vector<Particle::Particle>& getParticles() { return particles; }
    FluidSimulator();

private:
    
    bool isIndexOutOfBoundsOfGrid(int x_index, int y_index, int z_index);
    void findNeighbors(Particle::Particle& particle);
    void assignParticlesToGrid();
    void findAndSetNeighborsForAllParticles();
    void handleBoundaryCollision(Particle::Particle& particle);
    void computeDensityPressureForAllParticles();
    void computeForcesForAllParticles();
    void integrateAllParticles();
    void enforceBoundaryConditionOnAllParticles();
};
#endif