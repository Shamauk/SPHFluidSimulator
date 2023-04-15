#include "fluid.h"

void Fluid::createScene(short sceneNumber) {
    int maxNumParticlesInX = (this->maxX - this->minX) / this->particleRadius;
    int maxNumParticlesInY = (this->maxY - this->minY) / this->particleRadius;

    if (sceneNumber == 0) {
        // Do scene
        this->particles.push_back(Particle(glm::dvec2(0.0, 0.0), glm::dvec2(0)));
    }
}

void Fluid::updateParticleStates() {
    this->calculateDensity();
}

void Fluid::calculateDensity() {
    for (auto &pi : this->particles) {
        pi.density = 0;
        for (auto &pj : this->particles) {
            glm::dvec2 rij = pj.position - pi.position;
            pi.density = pj.mass * this->kernel.getKernelValue(rij);
        }
    }
}



