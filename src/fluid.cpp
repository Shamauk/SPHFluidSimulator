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
        std::cout << "initializing dam break" << std::endl;
        // Temporarily change maxX 
        this->setMaxX(this->getMinX() + 3 * this->getParticleRadius());

        // Create all particles
        maxNumParticlesInX = (this->maxX - this->minX) / this->PARTICLE_RADIUS;
        for (int j = 0; j < maxNumParticlesInY; j++) {
            for (int i = 0; i < maxNumParticlesInX; i++) {
                this->particles.push_back(Particle(glm::dvec2(this->getMinX() + i * this->getParticleRadius(), 
                    this->minY + j * this->getParticleRadius()), glm::dvec2(0.0)));
            }
        }

        // Setup heartbeat
        this->frameToHeartbeat = 500;
        this->curFrame = 1;
        this->heartbeat = [this]() {
            std::cout << "dam breaking" << std::endl;
            this->setMaxX(1.0);
            this->frameToHeartbeat = 0;
        };
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
    } else if (sceneNumber == 4) { // Drop
        std::cout << "Initializing drop" << std::endl;
        int sceneMinX = -5 * (this->SMOOTHING_LENGTH / 2.0);
        for (int j = 0; j < 10; j++) {
            for (int i = 0; i < 10; i++) {
                this->particles.push_back(Particle(glm::dvec2(sceneMinX + i * (this->SMOOTHING_LENGTH / 2.0 + 0.01), 
                this->getMinY() + j * (this->SMOOTHING_LENGTH / 2.0 + 0.01)), glm::dvec2(0.0)));
            }
        }
    } else {
        std::cout << "NO SUCH SCENE" << std::endl;
        return;
    }
}

void Fluid::updateParticleStates() {
    this->addSceneUpdateParticles();
    this->calculateDensity();
    this->calculateForces();
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
            pi.density += this->PARTICLE_MASS * this->kernel.poly6(rij);
        }
        std::cout << "Particle density: " << pi.density << std::endl;
    }
}

void Fluid::calculateForces() {
    for (auto& pi : particles) {
        calculatePressure(pi);
        std::cout << "Pressure: " << pi.pressure << std::endl;
        glm::dvec2 fpres = glm::dvec2(0.0);
        glm::dvec2 fvisc = glm::dvec2(0.0);

        for (auto& pj : particles) {
            if (&pi == &pj || pj.density == 0) continue;

            glm::dvec2 rij = pj.position - pi.position;
            double lengthR = glm::length(rij);
            if (lengthR > this->SMOOTHING_LENGTH) continue;

            std::cout << "rij: " << rij.x << "," << rij.y << std::endl;
            std::cout << "pj density: " << pj.density << std::endl;
            std::cout << "pi pressure: " << pi.pressure << std::endl;
            std::cout << "pj pressure: " << pj.pressure << std::endl;
            std::cout << "kernel spiky: " << kernel.spiky(rij) << std::endl;
            fpres -= (rij / lengthR) * this->PARTICLE_MASS * (pi.pressure + pj.pressure) / (2.0 * pj.density) * kernel.spiky(rij);
            std::cout << "Fpres: " << fpres.x << "," << fpres.y << std::endl;
            fvisc += this->VISCOSITY * this->PARTICLE_MASS * (pj.velocity - pi.velocity) / pj.density * kernel.laplacianViscosity(rij);
        }
        glm::dvec2 fgrav = glm::dvec2(0, -9.81 * this->PARTICLE_MASS / pi.density);
        std::cout << "Pressure force: (" << fpres.x << "," << fpres.y << ")" << std::endl; 
        pi.forceAccumulation = fpres + fvisc + fgrav;
    }
}

void Fluid::calculatePressure(Particle &p) {
   // p.pressure = this->PRESSURE_STIFFNESS * ((p.density / this->REST_DENSITY) - 1);
   p.pressure = this->PRESSURE_STIFFNESS * (p.density - this->REST_DENSITY);
}

void Fluid::updateParticleVelocityAndPosition() {
    for (auto &p: this->particles) {
        // p.velocity = p.intermVelocity + (this->dt / this->PARTICLE_MASS) * p.forceAccumulation;
        p.velocity = this->dt * p.forceAccumulation / p.density;
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



