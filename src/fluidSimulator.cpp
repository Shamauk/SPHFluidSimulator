#include "fluidSimulator.h"

#include <iostream>

void FluidSimulator::updateParticles() {
    findAndSetNeighborsForAllParticles();
    for (Particle::Particle& particle : particles) {
        computeDensity(particle);
    }
    for (Particle::Particle& particle : particles) {
        particle.partialVelocity = particle.velocity + 
            dt * (computeForceFromGravity(particle) + computeForceFromViscosity(particle)) / particle.mass;
        calculateDii(particle);
    }
    for (Particle::Particle& particle : particles) {
        computePartialDensity(particle);
        particle.iterationPressure = 0.5 * particle.density;
        calculateAii(particle);
    }
    for (Particle::Particle& particle : particles) {
        int l = 0;
        // for getAverageIterationalDensity may have to recompute density before doing it
        // while (getAverageIterationalDensity() - REST_DENSITY > THRESHOLD || l < 2) {
        while (l < 2) {
            for (Particle::Particle& particle : particles) {
                calculateSumDijPj(particle);
            }
            for (Particle::Particle& particle : particles) {
                computeNextIterationPressure(particle);
                particle.pressure = particle.iterationPressure;
            }
            l++;
        }

    }
    for (Particle::Particle& particle : particles) {
        // compute force by pressure
        particle.velocity = particle.partialVelocity + dt * particle.forceAccumulator / particle.mass;
        particle.position += dt * particle.velocity;
        handleBoundaryCollision(particle);
    }
}

void FluidSimulator::computeNextIterationPressure(Particle::Particle& particle) {
    double sum = 0;
    for (Particle::Particle* neighbor : particle.neighbors) {
        sum += neighbor->mass * glm::dot(particle.sumdijpj - neighbor->dii * neighbor->iterationPressure - 
        (neighbor->sumdijpj - particle.dii*particle.iterationPressure), Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
    }
    particle.iterationPressure = (1-RELAXATION_FACTOR)*particle.iterationPressure + (RELAXATION_FACTOR/particle.aii) * (REST_DENSITY - particle.partialDensity - sum);
}

void FluidSimulator::calculateSumDijPj(Particle::Particle& particle) {
    glm::vec<3,double> sum = glm::vec<3,double>(0.0);
    for (Particle::Particle* neighbor : particle.neighbors) {
        sum += (-neighbor->mass / (neighbor->density * neighbor->density))* 
            neighbor->iterationPressure * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    particle.sumdijpj = dt * dt * sum;
}

double FluidSimulator::getAverageIterationalDensity() {
    double avg_density = 0;
    for (Particle::Particle& particle : particles) {
        computeDensity(particle);
        avg_density += particle.density;
    }
    return avg_density / particles.size();
}

void FluidSimulator::calculateDii(Particle::Particle& particle) {
    glm::vec<3,double> sum = glm::vec<3,double>(0.0);
    for (Particle::Particle* neighbor : particle.neighbors) {
        sum += (-neighbor->mass / (particle.density * particle.density)) * 
            Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    particle.dii = dt * dt * sum;
}

void FluidSimulator::calculateAii(Particle::Particle& particle) {
    double sum = 0;
    for (Particle::Particle* neighbor : particle.neighbors) {
        sum += neighbor->mass * glm::dot(particle.dii + dt*dt*(particle.mass/(particle.density*particle.density))*Kernel::getGradientKernelValue(neighbor->position, particle.position, SMOOTHING_LENGTH),
            Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
    }
    particle.aii = sum;
}

void FluidSimulator::handleBoundaryCollision(Particle::Particle& particle) {
    if (particle.position.x <= minX || particle.position.x >= maxX) {
        particle.velocity.x *= -BOUNDARY_DAMPING;
        particle.position.x = glm::clamp(particle.position.x, minX, maxX);
    }
    if (particle.position.y <= minY || particle.position.y >= maxY) {
        particle.velocity.y *= -BOUNDARY_DAMPING;
        particle.position.x = glm::clamp(particle.position.x, minX, maxX);
    }
    if (particle.position.z <= minZ || particle.position.z >= maxZ) {
        particle.velocity.z *= -BOUNDARY_DAMPING;
        particle.position.x = glm::clamp(particle.position.x, minX, maxX);
    }
}

void FluidSimulator::computeDensity(Particle::Particle& particle) {
    double sum = 0;
    for (Particle::Particle *neighbor : particle.neighbors) {
        sum += neighbor->mass * Kernel::getKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    particle.density = sum;
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
            (glm::dot(diff,Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH))
                /(glm::dot(diff,diff)+0.01*SMOOTHING_LENGTH*SMOOTHING_LENGTH)) );
    }
    return particle.mass * VISCOSITY * 2.0 * result;
}

glm::vec<3,double> FluidSimulator::computeForceFromPressure(Particle::Particle& particle) {
    glm::vec<3,double> sum = glm::vec<3,double>(0.0);
    for (Particle::Particle *neighbor : particle.neighbors) {
        sum += neighbor->mass * ( (particle.pressure / (particle.density * particle.density)) + 
            (neighbor->pressure / (neighbor->density * neighbor->density))) * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
    }
    return -(particle.mass/particle.partialDensity) * particle.density * sum;
}


void FluidSimulator::findAndSetNeighborsForAllParticles() {
    for (Particle::Particle& particle : particles) {
        findNeighbors(particle);
    }
}

void FluidSimulator::findNeighbors(Particle::Particle& particle) {
    particle.neighbors.clear();
    for (const Particle::Particle& pj : particles) {
        if (&pj == &particle) continue;
        double dist = glm::distance(particle.position, pj.position);
        if (dist <= 2 * SMOOTHING_LENGTH) {
            particle.neighbors.push_back(const_cast<Particle::Particle*>(&pj));
        }
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
        relativeDensityChange += glm::dot(particle.partialVelocity - neighbor->partialVelocity, Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
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


void FluidSimulator::addParticleFromXYZ(double x, double y, double z) {
    Particle::Particle particle = Particle::ParticlefromXYZ(x, y, z);
    particle.mass = SMOOTHING_LENGTH*SMOOTHING_LENGTH*SMOOTHING_LENGTH*REST_DENSITY;
    particles.push_back(particle);
}
