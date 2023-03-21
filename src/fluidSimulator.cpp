#include "fluidSimulator.h"


void FluidSimulator::updateParticles() {
    setNeighbors();
    computePartialVelocities();
    computePartialDensities();
    computePressuresFromPoisson();
    computePressureForces();
    updateFinalVelocityAndPosition();
}

void FluidSimulator::addParticleFromXYZWithRestDensity(double x, double y, double z, double restDensity) {
    Particle::Particle particle = Particle::ParticlefromXYZ(x, y, z);
    particle.restDensity = restDensity;
    particle.mass = SMOOTHING_LENGTH*SMOOTHING_LENGTH*SMOOTHING_LENGTH*particle.restDensity;
}

glm::vec<3,double>FluidSimulator::computeForceFromGravity(Particle::Particle& particle) {
    return particle.mass * ACCELERATION_DUE_TO_GRAVITY;
}

glm::vec<3,double> FluidSimulator::computeForceFromViscosity(Particle::Particle& particle, double viscosity, double h) {
    glm::vec<3,double> result = glm::vec<3,double>(0.0);
    glm::vec<3,double> diff;
    for (Particle::Particle* neighbor : particle.neighbors) {
        diff = particle.position - neighbor->position;
        result += ( (neighbor->mass / neighbor->density) * (particle.velocity - neighbor->velocity) * 
            (glm::dot(diff,Kernel::getGradientKernelValue(particle.position, neighbor->position, h))/(glm::dot(diff,diff)+0.01*h*h)) );
    }
    return 2.0 * result;
}

std::vector<Particle::Particle*> FluidSimulator::findNeighbors(Particle::Particle& particle) {
    std::vector<Particle::Particle*> neighbors;
    for (const Particle::Particle& pj : particles) {
        if (&pj == &particle) continue;  // skip
        double dist = glm::distance(particle.position, pj.position);
        if (dist <= particle.radius) {
            neighbors.push_back(const_cast<Particle::Particle*>(&pj));  // store the pointer to the neighbor particle
        }
    }
    return neighbors;
}

void FluidSimulator::setNeighbors() {
    for (Particle::Particle& particle : particles) {
        particle.neighbors = findNeighbors(particle);
    }
}

void FluidSimulator::computePartialVelocities() {
    for (Particle::Particle& particle : particles) {
        glm::vec<3,double> F_visc = computeForceFromViscosity(particle, viscosity, SMOOTHING_LENGTH);
        glm::vec<3,double> F_grav = computeForceFromGravity(particle);
        particle.partialVelocity += dt * (F_visc + F_grav) / particle.mass;
    }
}

void FluidSimulator::computePartialDensity(Particle::Particle& particle) {
    double baseDensityFromMass = 0;
    double relativeDensityChange = 0;
    for (Particle::Particle* neighbor : particle.neighbors) {
        baseDensityFromMass += neighbor->mass * Kernel::getKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
        relativeDensityChange += glm::dot(particle.velocity - neighbor->velocity, Kernel::getGradientKernelValue(particle.position, neighbor->velocity, SMOOTHING_LENGTH));
    }
    particle.partialDensity = baseDensityFromMass + dt * relativeDensityChange;
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
