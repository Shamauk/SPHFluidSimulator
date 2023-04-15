#include "fluid.h"
#include <iostream>

void Fluid::createScene(short sceneNumber) {
    int maxNumParticlesInX = (this->maxX - this->minX) / this->PARTICLE_RADIUS;
    int maxNumParticlesInY = (this->maxY - this->minY) / this->PARTICLE_RADIUS;

    if (sceneNumber == 0) {
        // Do scene
        this->particles.push_back(Particle(glm::dvec2(0.0, 0.0), glm::dvec2(0)));
        this->particles.push_back(Particle(glm::dvec2(0.0, this->getSmoothingLength() / 2.0), glm::dvec2(0)));
        this->particles.push_back(Particle(glm::dvec2(this->getSmoothingLength() / 2.0, 0.0), glm::dvec2(0)));
        this->particles.push_back(Particle(glm::dvec2(-1 * (this->getSmoothingLength() / 2.0), 0.0), glm::dvec2(0)));
    } else if (sceneNumber == 1) { // DAM BREAK
        // std::cout << "initializing dam break with " << this->DAM_PARTICLES << " particles" << std::endl;
        // for (double y = this->minY; y < 0.5 * this->maxY; y += this->getSmoothingLength() / 2.0);
        // {
        //     for (double x = -0.2; x <= 0.2; x += this->getSmoothingLength() / 2.0)
        //     {
        //         if (particles.size() < DAM_PARTICLES)
        //         {
        //             double jitter = static_cast<float>(arc4random()) / static_cast<float>(RAND_MAX);
        //             particles.push_back(Particle(glm::dvec2(x + jitter, y), glm::dvec2(0.0)));
        //         }
        //         else
        //         {
        //             return;
        //         }
        //     }
        // }
    } else if (sceneNumber == 2) {
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 25; i++) {
                this->particles.push_back(Particle(glm::dvec2(i * this->PARTICLE_RADIUS, j * this->PARTICLE_RADIUS), glm::dvec2(0.0)));
            }
        }
    } else if (sceneNumber == 3) {
        this->frameToHeartbeat = 100;
        this->heartbeat = [this]() {
            this->particles.push_back(Particle(glm::dvec2((rand() % 10) / 50.0 ,0.0), glm::dvec2(0.0)));
        };
    } else {
        std::cout << "NO SUCH SCENE" << std::endl;
        return;
    }
}

void Fluid::updateParticleStates() {
    this->addSceneUpdateParticles();
    this->calculateDensity();
    this->calculateIntermediateVelocity();
    this->calculatePressureAndPressureForce();
    this->updateParticleVelocityAndPosition();
    this->handleBoundaryConditions();
}

void Fluid::addSceneUpdateParticles() {
    if (this->frameToHeartbeat <= 0) {
        return;
    }
    if (this->curFrame % this->frameToHeartbeat == 0) {
        this->heartbeat();
    }
    this->curFrame++;
}

void Fluid::calculateDensity() {
    for (auto &pi : this->particles) {
        pi.density = 0;
        for (auto &pj : this->particles) {
            glm::dvec2 rij = pi.position - pj.position;
            pi.density += this->PARTICLE_MASS * this->kernel.getKernelValue(rij);
        }
    }
}

void Fluid::calculateIntermediateVelocity() {
    for (auto &pi : this->particles) {
        glm::dvec2 tmp = glm::dvec2(0.0);
        for (auto &pj : this->particles) {
            if (&pi == &pj) continue;
            glm::dvec2 rij = pi.position - pj.position;
            double lengthRij = glm::length(rij);
            if (lengthRij == 0 || pj.density == 0) continue;
            tmp += (this->PARTICLE_MASS / pj.density) * (pi.velocity - pj.velocity) * 
                (2 / glm::length(rij)) * glm::length(kernel.getKernelGradient(rij));
        }
        glm::dvec2 forceViscosity = -1 * this->PARTICLE_MASS * this->VISCOSITY * tmp;
        glm::dvec2 forceGravity = glm::dvec2(0.0, -9.8 * this->PARTICLE_MASS);

        pi.intermVelocity = pi.velocity + (dt / this->PARTICLE_MASS) * (forceGravity + forceViscosity);
    }
}

void Fluid::calculatePressureAndPressureForce() {
    for (auto &pi : this->particles) {
        calculatePressure(pi);

        pi.forceAccumulation = glm::dvec2(0.0);
        for (auto &pj : this->particles) {
            if (&pi == &pj) continue;
            glm::dvec2 rij = pi.position - pj.position;
            if (pi.density == 0 || pj.density == 0) continue;
            pi.forceAccumulation -= this->PARTICLE_MASS * ((pi.pressure / (pi.density * pi.density)) 
                + (pj.pressure / (pj.density * pj.density))) * kernel.getKernelGradient(rij);
        }
    }
}

void Fluid::calculatePressure(Particle &p) {
    p.pressure = this->PRESSURE_STIFFNESS * ((p.density / this->REST_DENSITY) - 1);
}

void Fluid::updateParticleVelocityAndPosition() {
    for (auto &p: this->particles) {
        p.velocity = p.intermVelocity + (this->dt / this->PARTICLE_MASS) * p.forceAccumulation;
        p.position += this->dt * p.velocity;
    }
}

void Fluid::handleBoundaryConditions() {
    for (auto &p: this->particles) {
        if (p.position.x < this->minX) {
            p.position.x = this->minX;
            p.velocity.x *= BOUNDARY_DAMPING;
        } else if (p.position.x > this->maxX) {
            p.position.x = this->maxX;
            p.velocity.x *= this->BOUNDARY_DAMPING;
        }

        if (p.position.y < this->minY) {
            p.position.y = this->minY;
            p.velocity.y *= this->BOUNDARY_DAMPING;
        } else if (p.position.y > this->maxY) {
            p.position.y = this->maxY;
            p.velocity.y *= this->BOUNDARY_DAMPING;
        }
    }
}



