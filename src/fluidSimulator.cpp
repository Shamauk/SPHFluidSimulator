#include "fluidSimulator.h"

#include <iostream>

// void FluidSimulator::updateParticles() {
//     for (Particle::Particle particle : particles) {
//         findNeighbors(particle);
//     }
//     for (Particle::Particle particle : particles) {
//         computeDensity(particle);
//     }
//     for (Particle::Particle particle : particles) {
//         glm::vec<3,double> force = computeForceFromViscosity(particle) + computeForceFromGravity(particle);
//         particle.partialVelocity = particle.velocity + dt * force / particle.mass;
//         // what is dii??
//     }
//     for (Particle::Particle particle : particles) {
//         computePartialDensity(particle);
//         particle.pressure *= 0.5; // Guess
//     }
// }

void FluidSimulator::computeDensity(Particle::Particle& particle) {
    int sum = 0;
    for (Particle::Particle *neighbor : particle.neighbors) {
        sum += neighbor->mass * Kernel::getKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    particle.density = sum;
}

// Attempted ISPH
// void FluidSimulator::updateParticles() {
//     getNeighbors();
//     computePartialVelocities();
//     computePartialDensities();
//     computePressuresFromPoisson();
//     computePressureForces();
//     updateFinalVelocityAndPosition();
// }

void FluidSimulator::updateParticles() {
    getNeighbors();
    for (Particle::Particle& particle : particles) {
        computeDensity(particle);
        double tmp = particle.density / particle.restDensity;
        particle.pressure = 0.5 * (tmp*tmp*tmp*tmp*tmp*tmp*tmp-1);
    }
    for (Particle::Particle& particle : particles) {
        particle.forceAccumulator = computeForceFromGravity(particle) + 
            computeForceFromViscosity(particle) + computeForceFromPressure(particle);
        std::cout << "fx,fy,fz" << particle.forceAccumulator.x << ","  << particle.forceAccumulator.y << "," << particle.forceAccumulator.z << std::endl;
    }
    for (Particle::Particle& particle : particles) {
        std::cout << "fx,fy,fz" << particle.forceAccumulator.x << ","  << particle.forceAccumulator.y << "," << particle.forceAccumulator.z << std::endl;
        particle.velocity += dt * particle.forceAccumulator / particle.mass;
        particle.position += dt * particle.velocity;
    }
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

std::vector<Particle::Particle*> FluidSimulator::findNeighbors(Particle::Particle& particle) {
    std::vector<Particle::Particle*> neighbors;
    for (const Particle::Particle& pj : particles) {
        if (&pj == &particle) continue;  // skip
        double dist = glm::distance(particle.position, pj.position);
        if (dist <= 2 * SMOOTHING_LENGTH) {
            neighbors.push_back(const_cast<Particle::Particle*>(&pj));  // store the pointer to the neighbor particle
        }
    }
    return neighbors;
}

void FluidSimulator::getNeighbors() {
    for (Particle::Particle& particle : particles) {
        particle.neighbors = findNeighbors(particle);
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
