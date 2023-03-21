#include "particle.h"

namespace Particle {
    Particle ParticlefromXYZ(double x, double y, double z) {
        Particle particle;
        particle.position = glm::vec<3,double>(x,y,z);
        return particle;
    }
}