#include "fluidSimulator.h"

#include <iostream>


void FluidSimulator::updateParticles() {
    findAndSetNeighborsForAllParticles();
    for (Particle::Particle& particle : particles) {
       // std::cout << particle.position.x << "," << particle.position.y << "," << particle.position.z << std::endl;
        computeDensity(particle);
      //  std::cout << particle.density << std::endl;
        double tmp = particle.density / particle.restDensity;
        particle.pressure = 0.05 * (tmp*tmp*tmp*tmp*tmp*tmp*tmp-1);
    }
    for (Particle::Particle& particle : particles) {
        particle.forceAccumulator = computeForceFromGravity(particle) + 
            computeForceFromViscosity(particle) + computeForceFromPressure(particle);
       // glm::vec<3,double> tmp = computeForceFromViscosity(particle);
      //  std::cout << tmp.x << "," << tmp.y << "," << tmp.z << std::endl;
       // std::cout << particle.forceAccumulator.x << "," << particle.forceAccumulator.y << "," << particle.forceAccumulator.z << std::endl;
    }
    for (Particle::Particle& particle : particles) {
        particle.velocity += dt * particle.forceAccumulator / particle.mass;
        particle.position += dt * particle.velocity;
       // std::cout << particle.position.x << "," << particle.position.y << "," << particle.position.z << std::endl;
        handleBoundaryCollision(particle);
    }
}

void FluidSimulator::handleBoundaryCollision(Particle::Particle& particle) {
    if (particle.position.x <= minX || particle.position.x >= maxX) {
        particle.velocity *= -boundaryDamping;
        particle.position.x = glm::clamp(particle.position.x, minX, maxX);
    }
    if (particle.position.y <= minY || particle.position.y >= maxY) {
        particle.velocity *= -boundaryDamping;
        particle.position.x = glm::clamp(particle.position.x, minX, maxX);
    }
    if (particle.position.z <= minZ || particle.position.z >= maxZ) {
        particle.velocity *= -boundaryDamping;
        particle.position.x = glm::clamp(particle.position.x, minX, maxX);
    }
}

void FluidSimulator::computeDensity(Particle::Particle& particle) {
    double sum = 0;
    for (Particle::Particle *neighbor : particle.neighbors) {
        sum += neighbor->mass * Kernel::getKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    std::cout << sum << std::endl;
    particle.density = sum;
}

glm::vec<3,double> FluidSimulator::computeForceFromPressure(Particle::Particle& particle) {
    glm::vec<3,double> sum = glm::vec<3,double>(0.0);
    for (Particle::Particle *neighbor : particle.neighbors) {
        sum += neighbor->mass * (particle.pressure / (particle.density * particle.density) + 
            neighbor->pressure / (neighbor->density * neighbor->density)) * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    return -particle.mass * sum;
}

void FluidSimulator::addParticleFromXYZWithRestDensity(double x, double y, double z, double restDensity) {
    Particle::Particle particle = Particle::ParticlefromXYZ(x, y, z);
    particle.restDensity = restDensity;
    particle.mass = SMOOTHING_LENGTH*SMOOTHING_LENGTH*SMOOTHING_LENGTH*particle.restDensity;
    particles.push_back(particle);
}

glm::vec<3,double>FluidSimulator::computeForceFromGravity(Particle::Particle& particle) {
    return particle.mass * ACCELERATION_DUE_TO_GRAVITY;
}

glm::vec<3,double> FluidSimulator::computeForceFromViscosity(Particle::Particle& particle) {
    glm::vec<3,double> result = glm::vec<3,double>(0.0);
    glm::vec<3,double> diff;
    for (Particle::Particle* neighbor : particle.neighbors) {
        diff = particle.position - neighbor->position;
        result += ( (neighbor->mass / neighbor->density) * (particle.velocity - neighbor->velocity) * 
            (glm::dot(diff,Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH))/(glm::dot(diff,diff)+0.01*SMOOTHING_LENGTH*SMOOTHING_LENGTH)) );
    }
    return 2.0 * result;
}

void FluidSimulator::findNeighbors(Particle::Particle& particle) {
    particle.neighbors.clear();
    for (const Particle::Particle& pj : particles) {
        if (&pj == &particle) continue;  // skip
        double dist = glm::distance(particle.position, pj.position);
        if (dist <= 2 * SMOOTHING_LENGTH) {
            particle.neighbors.push_back(const_cast<Particle::Particle*>(&pj));  // store the pointer to the neighbor particle
        }
    }
}

void FluidSimulator::findAndSetNeighborsForAllParticles() {
    for (Particle::Particle& particle : particles) {
        findNeighbors(particle);
    }
}

void FluidSimulator::computePartialVelocities() {
    for (Particle::Particle& particle : particles) {
        glm::vec<3,double> F_visc = computeForceFromViscosity(particle);
        glm::vec<3,double> F_grav = computeForceFromGravity(particle);
        particle.partialVelocity += dt * (F_visc + F_grav) / particle.mass;
    }
}

void FluidSimulator::computePartialDensity(Particle::Particle& particle) {
    double relativeDensityChange = 0;
    for (Particle::Particle* neighbor : particle.neighbors) {
        relativeDensityChange += neighbor->mass * glm::dot(particle.velocity - neighbor->velocity, Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
    }
    particle.partialDensity = particle.density + dt * relativeDensityChange;
}

void FluidSimulator::computePartialDensities() {
    for (Particle::Particle& particle : particles) {
        computePartialDensity(particle);
    }
}

void FluidSimulator::computePressuresFromPoisson() {
    return;
}

void FluidSimulator::computePressureForceOnParticle(Particle::Particle& particle) {
    glm::vec<3,double> sum = glm::vec<3,double>(0.0);
    for (Particle::Particle* neighbor : particle.neighbors) {
        sum += neighbor->mass * (particle.pressure / (particle.density*particle.density) + 
            neighbor->pressure / (neighbor->density*neighbor->density)) * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    particle.forcePressure = - (particle.mass / particle.partialDensity) * particle.density * sum;
}

void FluidSimulator::computePressureForces() {
    for (Particle::Particle& particle : particles) {
        computePressureForceOnParticle(particle);
    }
}

void FluidSimulator::updateFinalVelocityAndPosition() {
    for (Particle::Particle& particle : particles) {
        particle.velocity = particle.partialVelocity + dt * particle.forcePressure / particle.mass;
        particle.position += dt * particle.velocity;
    }
}
