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

    for (Particle::Particle& particle : particles) {
        int x_pos = glm::clamp((int) ((particle.position.x - minX) / SMOOTHING_LENGTH), 0, numXGrids-1);
        int y_pos = glm::clamp((int) ((particle.position.y - minY) / SMOOTHING_LENGTH), 0, numYGrids-1);
        int z_pos = glm::clamp((int) ((particle.position.z - minZ) / SMOOTHING_LENGTH), 0, numZGrids-1);
        grid[x_pos][y_pos][z_pos].push_back(&particle);
    }
}

void FluidSimulator::findAndSetNeighborsForAllParticles() {
    for (Particle::Particle& particle : particles) {
        findNeighbors(particle);
    }
}

void FluidSimulator::findNeighbors(Particle::Particle& particle) {
    particle.neighbors.clear();
    int x_pos = (int) ((particle.position.x - minX) / SMOOTHING_LENGTH);
    int y_pos = (int) ((particle.position.y - minY) / SMOOTHING_LENGTH);
    int z_pos = (int) ((particle.position.z - minZ) / SMOOTHING_LENGTH);

    // int minIIndex = x_pos - 1 < 0 ? 0 : -1;
    // int minJIndex = y_pos - 1 < 0 ? 0 : -1;
    // int minKIndex = z_pos - 1 < 0 ? 0 : -1;
    // int maxIIndex = x_pos + 1 >= numXGrids ? 0 : 1;
    // int maxJIndex = y_pos + 1 >= numYGrids ? 0 : 1;
    // int maxKIndex = z_pos + 1 >= numZGrids ? 0 : 1;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            for (int k = -1; k <= 1; k++) {
                if (i+x_pos < 0 || i+x_pos >= numXGrids || j+y_pos < 0 || j+y_pos >= numYGrids || k+z_pos < 0 || k+z_pos >= numZGrids) continue;
                for (Particle::Particle* potentialParticle : grid[i+x_pos][j+y_pos][k+z_pos]) {
                    if (potentialParticle == &particle) continue;
                    if (glm::distance(particle.position, potentialParticle->position) <= SMOOTHING_LENGTH) {
                        particle.neighbors.push_back(potentialParticle);
                    }
                }
            }
        }
    }
}

// void FluidSimulator::findNeighbors(Particle::Particle& particle) {
//     particle.neighbors.clear();
//     for (const Particle::Particle& pj : particles) {
//         if (&pj == &particle) continue;
//         double dist = glm::distance(particle.position, pj.position);
//         if (dist <= SMOOTHING_LENGTH) {
//             particle.neighbors.push_back(const_cast<Particle::Particle*>(&pj));
//         }
//     }
// }



void FluidSimulator::computeDensityPressureForAllParticles() {
    for (Particle::Particle& particle : particles) {
        particle.density = particle.mass;
        for (Particle::Particle* neighbor : particle.neighbors) {
            glm::dvec3 r = particle.position - neighbor->position;
            particle.density += neighbor->mass * Kernel::poly6Kernel(r, SMOOTHING_LENGTH);
        }
        double weight = particle.density / REST_DENSITY;
        particle.pressure = STIFFNESS * (weight*weight*weight*weight*weight*weight*weight - 1.0);
    }
}

void FluidSimulator::computeForcesForAllParticles() {
    for (Particle::Particle& particle : particles) {
        glm::dvec3 pressureForce = glm::dvec3(0.0);
        glm::dvec3 viscosityForce = glm::dvec3(0.0);

        for (Particle::Particle* neighbor : particle.neighbors) {
            glm::dvec3 r = particle.position - neighbor->position;
            pressureForce -= neighbor->mass * ((particle.pressure + neighbor->pressure) / (2*neighbor->density)) * Kernel::spikyKernelGradient(r, SMOOTHING_LENGTH);
            viscosityForce += neighbor->mass * ((neighbor->velocity - particle.velocity)/neighbor->density) * Kernel::viscosityKernelLaplacian(r, SMOOTHING_LENGTH);
        }

        viscosityForce *= VISCOSITY;

        glm::dvec3 gravityForce = particle.mass * ACCELERATION_DUE_TO_GRAVITY;
      particle.forceAccumulator = pressureForce + viscosityForce + gravityForce;
    }
}

void FluidSimulator::integrateAllParticles() {
    for (Particle::Particle& particle : particles) {
        particle.velocity += dt * (particle.forceAccumulator / particle.mass);
        particle.position += dt * particle.velocity;
    }
}

void FluidSimulator::enforceBoundaryConditionOnAllParticles() {
    for (Particle::Particle& particle : particles) {
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

// void FluidSimulator::handleBoundaryCollision(Particle::Particle& particle) {
//     if (particle.position.x <= minX || particle.position.x >= maxX) {
//         particle.velocity.x *= -BOUNDARY_DAMPING;
//         particle.position.x = glm::clamp(particle.position.x, (double) minX, (double) maxX);
//     }
//     if (particle.position.y <= minY || particle.position.y >= maxY) {
//         particle.velocity.y *= -BOUNDARY_DAMPING;
//         particle.position.y = glm::clamp(particle.position.y, (double) minY, (double) maxY);
//     }
//     if (particle.position.z <= minZ || particle.position.z >= maxZ) {
//         particle.velocity.z *= -BOUNDARY_DAMPING;
//         particle.position.z = glm::clamp(particle.position.z, (double) minZ, (double) maxZ);
//     }
// }

// void FluidSimulator::updateParticles() {
//     findAndSetNeighborsForAllParticles();
//     for (Particle::Particle& particle : particles) {
//         computeDensity(particle);
//     }
//     for (Particle::Particle& particle : particles) {
//         particle.partialVelocity = particle.velocity + 
//             dt * (computeForceFromGravity(particle)) / particle.mass; // + computeForceFromViscosity(particle)
//         calculateDii(particle);
//     }
//     for (Particle::Particle& particle : particles) {
//         computePartialDensity(particle);
//         particle.iterationPressure = 0.5 * particle.density;
//         calculateAii(particle);
//     }
//     for (Particle::Particle& particle : particles) {
//         int l = 0;
//         // for getAverageIterationalDensity may have to recompute density before doing it
//         // while (getAverageIterationalDensity() - REST_DENSITY > THRESHOLD || l < 2) {
//         while (l < 2) {
//             for (Particle::Particle& particle : particles) {
//                 calculateSumDijPj(particle);
//             }
//             for (Particle::Particle& particle : particles) {
//                 computeNextIterationPressure(particle);
//                 particle.pressure = particle.iterationPressure;
//             }
//             l++;
//         }

//     }
//     for (Particle::Particle& particle : particles) {
//         // compute force by pressure
//         particle.velocity = particle.partialVelocity + dt * computeForceFromPressure(particle) / particle.mass;
//         particle.position += dt * particle.velocity;
//         handleBoundaryCollision(particle);
//     }
// }

// void FluidSimulator::computeNextIterationPressure(Particle::Particle& particle) {
//     double sum = 0;
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         sum += neighbor->mass * glm::dot(particle.sumdijpj - neighbor->dii * neighbor->iterationPressure - 
//         (neighbor->sumdijpj - particle.dii*particle.iterationPressure), Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
//     }
//     particle.iterationPressure = (1-RELAXATION_FACTOR)*particle.iterationPressure + (RELAXATION_FACTOR/particle.aii) * (REST_DENSITY - particle.partialDensity - sum);
// }

// void FluidSimulator::calculateSumDijPj(Particle::Particle& particle) {
//     glm::vec<3,double> sum = glm::vec<3,double>(0.0);
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         sum += (-neighbor->mass / (neighbor->density * neighbor->density))* 
//             neighbor->iterationPressure * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
//     }
//     particle.sumdijpj = dt * dt * sum;
// }

// double FluidSimulator::getAverageIterationalDensity() {
//     double avg_density = 0;
//     for (Particle::Particle& particle : particles) {
//         computeDensity(particle);
//         avg_density += particle.density;
//     }
//     return avg_density / particles.size();
// }

// void FluidSimulator::calculateDii(Particle::Particle& particle) {
//     glm::vec<3,double> sum = glm::vec<3,double>(0.0);
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         sum += (-neighbor->mass / (particle.density * particle.density)) * 
//             Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
//     }
//     particle.dii = dt * dt * sum;
// }

// void FluidSimulator::calculateAii(Particle::Particle& particle) {
//     double sum = 0;
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         sum += neighbor->mass * glm::dot(particle.dii + dt*dt*(particle.mass/(particle.density*particle.density))*Kernel::getGradientKernelValue(neighbor->position, particle.position, SMOOTHING_LENGTH),
//             Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
//     }
//     particle.aii = sum;
// }

// void FluidSimulator::handleBoundaryCollision(Particle::Particle& particle) {
//     if (particle.position.x <= minX || particle.position.x >= maxX) {
//         particle.velocity.x *= -BOUNDARY_DAMPING;
//         particle.position.x = glm::clamp(particle.position.x, minX, maxX);
//     }
//     if (particle.position.y <= minY || particle.position.y >= maxY) {
//         particle.velocity.y *= -BOUNDARY_DAMPING;
//         particle.position.y = glm::clamp(particle.position.y, minY, maxY);
//     }
//     if (particle.position.z <= minZ || particle.position.z >= maxZ) {
//         particle.velocity.z *= -BOUNDARY_DAMPING;
//         particle.position.z = glm::clamp(particle.position.z, minZ, maxZ);
//     }
// }

// void FluidSimulator::computeDensity(Particle::Particle& particle) {
//     double sum = 0;
//     for (Particle::Particle *neighbor : particle.neighbors) {
//         sum += neighbor->mass * Kernel::getKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
//     }
//     particle.density = sum;
// }

// glm::vec<3,double>FluidSimulator::computeForceFromGravity(Particle::Particle& particle) {
//     return particle.mass * ACCELERATION_DUE_TO_GRAVITY;
// }

// glm::vec<3,double> FluidSimulator::computeForceFromViscosity(Particle::Particle& particle) {
//     glm::vec<3,double> result = glm::vec<3,double>(0.0);
//     glm::vec<3,double> diff;
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         diff = particle.position - neighbor->position;
//         result += ( (neighbor->mass / neighbor->density) * (particle.velocity - neighbor->velocity) * 
//             (glm::dot(diff,Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH))
//                 /(glm::dot(diff,diff)+0.01*SMOOTHING_LENGTH*SMOOTHING_LENGTH)) );
//     }
//     return particle.mass * VISCOSITY * 2.0 * result;
// }

// glm::vec<3,double> FluidSimulator::computeForceFromPressure(Particle::Particle& particle) {
//     glm::vec<3,double> sum = glm::vec<3,double>(0.0);
//     for (Particle::Particle *neighbor : particle.neighbors) {
//         sum += neighbor->mass * ( (particle.pressure / (particle.density * particle.density)) + 
//             (neighbor->pressure / (neighbor->density * neighbor->density))) * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
//     }
//     return -particle.mass * sum;
// }


// void FluidSimulator::findAndSetNeighborsForAllParticles() {
//     for (Particle::Particle& particle : particles) {
//         findNeighbors(particle);
//     }
// }

// void FluidSimulator::findNeighbors(Particle::Particle& particle) {
//     particle.neighbors.clear();
//     for (const Particle::Particle& pj : particles) {
//         if (&pj == &particle) continue;
//         double dist = glm::distance(particle.position, pj.position);
//         if (dist <= 2 * SMOOTHING_LENGTH) {
//             particle.neighbors.push_back(const_cast<Particle::Particle*>(&pj));
//         }
//     }
// }

// void FluidSimulator::computePartialVelocities() {
//     for (Particle::Particle& particle : particles) {
//         glm::vec<3,double> F_visc = computeForceFromViscosity(particle);
//         glm::vec<3,double> F_grav = computeForceFromGravity(particle);
//         particle.partialVelocity += dt * (F_visc + F_grav) / particle.mass;
//     }
// }

// void FluidSimulator::computePartialDensity(Particle::Particle& particle) {
//     double relativeDensityChange = 0;
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         relativeDensityChange += glm::dot(particle.partialVelocity - neighbor->partialVelocity, Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH));
//     }
//     particle.partialDensity = particle.density + dt * relativeDensityChange;
// }

// void FluidSimulator::computePartialDensities() {
//     for (Particle::Particle& particle : particles) {
//         computePartialDensity(particle);
//     }
// }

// void FluidSimulator::computePressuresFromPoisson() {
//     return;
// }

// void FluidSimulator::computePressureForceOnParticle(Particle::Particle& particle) {
//     glm::vec<3,double> sum = glm::vec<3,double>(0.0);
//     for (Particle::Particle* neighbor : particle.neighbors) {
//         sum += neighbor->mass * (particle.pressure / (particle.density*particle.density) + 
//             neighbor->pressure / (neighbor->density*neighbor->density)) * Kernel::getGradientKernelValue(particle.position, neighbor->position, SMOOTHING_LENGTH);
//     }
//     particle.forcePressure = - (particle.mass / particle.partialDensity) * particle.density * sum;
// }

// void FluidSimulator::computePressureForces() {
//     for (Particle::Particle& particle : particles) {
//         computePressureForceOnParticle(particle);
//     }
// }

// void FluidSimulator::updateFinalVelocityAndPosition() {
//     for (Particle::Particle& particle : particles) {
//         particle.velocity = particle.partialVelocity + dt * particle.forcePressure / particle.mass;
//         particle.position += dt * particle.velocity;
//     }
// }

// void FluidSimulator::addParticleFromXYZ(double x, double y, double z) {
//     Particle::Particle particle = Particle::ParticlefromXYZ(x, y, z);
//     particle.mass = SMOOTHING_LENGTH*SMOOTHING_LENGTH*SMOOTHING_LENGTH*REST_DENSITY;
//     particles.push_back(particle);
// }
