#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <glm/glm.hpp>
#include <vector>

namespace Particle {
    struct Particle {
        double mass = 1;
        double density;
        double pressure;
        glm::dvec3 position = glm::dvec3(0.0);
        glm::dvec3 velocity = glm::dvec3(0.0);

        glm::dvec3 forceAccumulator;

        std::vector<Particle*> neighbors;

        glm::dvec3 initialForce = glm::dvec3(0.0);
    };

    Particle ParticlefromXYZ(double x, double y, double z);
}

#endif