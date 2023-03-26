#include "fluidSimulator.h"

#include <iostream>

void FluidSimulator::updateParticles() {
    assignParticlesToGrid();
    findAndSetNeighborsForAllParticles();
    computeDensityPressureForAllParticles();
    computeForcesForAllParticles();
    integrateAllParticles();
    enforceBoundaryConditionOnAllParticles();
}

void FluidSimulator::assignParticlesToGrid() {
    for (int x = 0; x < numXGrids; x++) {
        for (int y = 0; y < numYGrids; y++) {
            for (int z = 0; z < numZGrids; z++) {
                grid[x][y][z].clear();
            }
        }
    }

    for (auto& particle : particles) {
        int x_pos = glm::clamp((int) ((particle.position.x - minX) / SMOOTHING_LENGTH), 0, numXGrids-1);
        int y_pos = glm::clamp((int) ((particle.position.y - minY) / SMOOTHING_LENGTH), 0, numYGrids-1);
        int z_pos = glm::clamp((int) ((particle.position.z - minZ) / SMOOTHING_LENGTH), 0, numZGrids-1);
        grid[x_pos][y_pos][z_pos].push_back(&particle);
    }
}

void FluidSimulator::findAndSetNeighborsForAllParticles() {
    for (auto& particle : particles) {
        findNeighbors(particle);
    }
}

void FluidSimulator::findNeighbors(Particle::Particle& particle) {
    particle.neighbors.clear();
    int x_pos = (int) ((particle.position.x - minX) / SMOOTHING_LENGTH);
    int y_pos = (int) ((particle.position.y - minY) / SMOOTHING_LENGTH);
    int z_pos = (int) ((particle.position.z - minZ) / SMOOTHING_LENGTH);

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                if (isIndexOutOfBoundsOfGrid(i+x_pos, j+y_pos, k+z_pos))
                    continue;
                for (auto* potentialParticle : grid[i+x_pos][j+y_pos][k+z_pos]) {
                    if (potentialParticle == &particle) continue;
                    if (glm::distance(particle.position, potentialParticle->position) <= SMOOTHING_LENGTH) {
                        particle.neighbors.push_back(potentialParticle);
                    }
                }
            }
        }
    }
}

inline bool FluidSimulator::isIndexOutOfBoundsOfGrid(int x_index, int y_index, int z_index) {
    return (x_index < 0 || x_index >= numXGrids || y_index < 0 || y_index >= numYGrids || z_index < 0 
        || z_index >= numZGrids);
}

void FluidSimulator::computeDensityPressureForAllParticles() {
    glm::dvec3 zeroVec = glm::dvec3(0.0);
    for (auto& particle : particles) {
        particle.density = particle.mass * Kernel::poly6Kernel(zeroVec, SMOOTHING_LENGTH);
        for (auto* neighbor : particle.neighbors) {
            glm::dvec3 r = particle.position - neighbor->position;
            particle.density += neighbor->mass * Kernel::poly6Kernel(r, SMOOTHING_LENGTH);
        }
        double weight = particle.density / REST_DENSITY;
        particle.pressure = STIFFNESS * (weight*weight*weight*weight*weight*weight*weight - 1.0);
    }
}

void FluidSimulator::computeForcesForAllParticles() {
    for (auto& particle : particles) {
        glm::dvec3 pressureForce = glm::dvec3(0.0);
        glm::dvec3 viscosityForce = glm::dvec3(0.0);

        for (auto* neighbor : particle.neighbors) {
            glm::dvec3 r = particle.position - neighbor->position;
            pressureForce -= neighbor->mass * ((particle.pressure + neighbor->pressure) / (2.0*neighbor->density)) * Kernel::spikyKernelGradient(r, SMOOTHING_LENGTH);
            viscosityForce += neighbor->mass * ((particle.velocity - neighbor->velocity)/neighbor->density) * Kernel::viscosityKernelLaplacian(r, SMOOTHING_LENGTH);
        }

        viscosityForce *= VISCOSITY;

        glm::dvec3 gravityForce = particle.mass * ACCELERATION_DUE_TO_GRAVITY;
        particle.forceAccumulator = pressureForce + viscosityForce + gravityForce + particle.initialForce;
        particle.initialForce = glm::dvec3(0.0);
    }
}

void FluidSimulator::integrateAllParticles() {
    for (auto& particle : particles) {
        particle.velocity += dt * (particle.forceAccumulator / particle.density);
        particle.position += dt * particle.velocity;
    }
}

void FluidSimulator::enforceBoundaryConditionOnAllParticles() {
    for (auto& particle : particles) {
        if (particle.position.x < minX) { particle.velocity.x = BOUNDARY_RESTITUTION * (minX - particle.position.x) / dt; particle.position.x = minX; }
        if (particle.position.x > maxX) { particle.velocity.x = BOUNDARY_RESTITUTION * (maxX - particle.position.x) / dt; particle.position.x = maxX; }
        if (particle.position.y < minY) { particle.velocity.y = BOUNDARY_RESTITUTION * (minY - particle.position.y) / dt; particle.position.y = minY; }
        if (particle.position.y > maxY) { particle.velocity.y = BOUNDARY_RESTITUTION * (maxY - particle.position.y) / dt; particle.position.y = maxY; }
        if (particle.position.z < minZ) { particle.velocity.z = BOUNDARY_RESTITUTION * (minZ - particle.position.z) / dt; particle.position.z = minZ; }
        if (particle.position.z > maxZ) { particle.velocity.z = BOUNDARY_RESTITUTION * (maxZ - particle.position.z) / dt; particle.position.z = maxZ; }
    }
}

void FluidSimulator::addParticleFromXYZ(double x, double y, double z) {
    if (x < minX || x > maxX || y < minY || y > maxY || z < minZ || z > maxZ) return;
    Particle::Particle particle = Particle::ParticlefromXYZ(x, y, z);
    particle.mass = SMOOTHING_LENGTH*SMOOTHING_LENGTH*SMOOTHING_LENGTH*REST_DENSITY;
    particles.push_back(particle);
}

void FluidSimulator::addParticle(Particle::Particle& particle) {
    if (particle.position.x < minX || particle.position.x > maxX || 
        particle.position.y < minY || particle.position.y > maxY || 
        particle.position.z < minZ || particle.position.z > maxZ) return;
    particle.mass = SMOOTHING_LENGTH*SMOOTHING_LENGTH*SMOOTHING_LENGTH*REST_DENSITY;
    particles.push_back(particle);
}

FluidSimulator::FluidSimulator() {
    numXGrids = (int)((maxX - minX) / SMOOTHING_LENGTH);
    numYGrids = (int)((maxY - minY) / SMOOTHING_LENGTH);
    numZGrids = (int)((maxZ - minZ) / SMOOTHING_LENGTH);

    grid = new std::vector<Particle::Particle*>**[numXGrids];
    for (int i = 0; i < numXGrids; i++) {
        grid[i] = new std::vector<Particle::Particle*>*[numYGrids];
        for (int j = 0; j < numYGrids; j++) {
            grid[i][j] = new std::vector<Particle::Particle*>[numZGrids];
        }
    }

    for (int i = 0; i < numXGrids; i++) {
        for (int j = 0; j < numYGrids; j++) {
            for (int k = 0; k < numZGrids; k++) {
                grid[i][j][k] = std::vector<Particle::Particle*>();
            }
        }
    }
}