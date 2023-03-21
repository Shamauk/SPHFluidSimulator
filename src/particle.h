#ifndef PARTICLE_H
#define PARTICLE_H

#include <cmath>
#include <glm/glm.hpp>
#include <vector>

namespace Particle {
    struct Particle {
        double mass = 1;
        double density;
        double restDensity;
        double pressure;
        double radius;
        glm::vec<3,double> position = glm::vec<3,double>(0.0);
        glm::vec<3,double> velocity = glm::vec<3,double>(0.0);

        glm::vec<3,double> partialVelocity;
        glm::vec<3,double> forcePressure;
        double partialDensity;

        std::vector<Particle*> neighbors;
    };

    Particle ParticlefromXYZ(double x, double y, double z);
}

#endif