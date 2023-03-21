#include <cmath>
#include <glm/glm.hpp>

class Particle {
    public:
        glm::vec<3,double> position;
        glm::vec<3,double> velocity;
        glm::vec<3,double> partialVelocity;
        glm::vec<3,double> forcePressure;
        double mass;
        double density;
        double partialDensity;
        double pressure;

        std::vector<Particle*> neighbors;

        static Particle fromXYZ(double x, double y, double z) {
            Particle particle;
            particle.position = glm::vec<3,double>(x,y,z);
            return particle;
        }

        std::vector<Particle*> findNeighbors(const std::vector<Particle>& particles, double radius);
        void computeForces(const std::vector<Particle*>& neighbors, double nu);
        void computeIntermediateDensity(const std::vector<Particle*>& neighbors, double dt);
        void Particle::solvePPE(const std::vector<Particle*>& neighbors, double rho0, double dt, int max_iterations, double tolerance);
};

double cubicSpline(double q) {
    double result = 3 / (2 * M_PI);
    if (q < 1) {
        result *= (2/3 - q*q + 0.5*q*q*q);
    } else if (q < 2) {
        result *= (1/6 * (2-q) * (2-q) * (2-q));
    } else {
        result *= 0;
    }
    return result;
}

double kernel(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
    double dist = glm::distance(xi, xj);
    return (cubicSpline(dist/h)) / (h*h*h); 
}

double gradCubicSpline(double q) {
    double result = 3 / (2 * M_PI);
    if (q < 1) {
        result *= (-2 * q  +  1.5 * q * q);
    } else if (q < 2) {
        result *= (-0.5 * (2-q) * (2-q));
    } else {
        result = 0;
    }
    return result;
}

glm::vec<3,double> gradKernel(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
    double dist = glm::distance(xi, xj);
    return (gradCubicSpline(dist/h)*((xi-xj)/dist)) / (h*h*h);
}

// // Simple Euclidean distance function
// double distance(const Particle& p1, const Particle& p2) {
//     double dx = p1.x - p2.x;
//     double dy = p1.y - p2.y;
//     double dz = p1.z - p2.z;
//     return std::sqrt(dx * dx + dy * dy + dz * dz);
// }



// void Particle::computeForces(const std::vector<Particle*>& neighbors, double nu) {
//     double f_viscosity_x = 0, f_viscosity_y = 0, f_viscosity_z = 0;
//     double f_gravity_x = 0, f_gravity_y = -mass * 9.81, f_gravity_z = 0;
//     for (const Particle* pj : neighbors) {
//         double dx = x - pj->x;
//         double dy = y - pj->y;
//         double dz = z - pj->z;
//         double r = std::sqrt(dx * dx + dy * dy + dz * dz);

//         double gradW_x, gradW_y, gradW_z;
//         gradKernel(r, dx, dy, dz, &gradW_x, &gradW_y, &gradW_z);  // Compute the gradient of the kernel function

//         double dvx = vx - pj->vx;
//         double dvy = vy - pj->vy;
//         double dvz = vz - pj->vz;

//         f_viscosity_x += pj->mass * nu * dvx * gradW_x;
//         f_viscosity_y += pj->mass * nu * dvy * gradW_y;
//         f_viscosity_z += pj->mass * nu * dvz * gradW_z;
//     }

//     this->vx += dt * (f_viscosity_x + f_gravity_x) / mass;
//     this->vy += dt * (f_viscosity_y + f_gravity_y) / mass;
//     this->vz += dt * (f_viscosity_z + f_gravity_z) / mass;
// }

// void Particle::computeIntermediateDensity(const std::vector<Particle*>& neighbors, double dt) {
//     double density = 0;

//     for (const Particle* pj : neighbors) {
//         double dx = x - pj->x;
//         double dy = y - pj->y;
//         double dz = z - pj->z;
//         double r = std::sqrt(dx * dx + dy * dy + dz * dz);

//         double W = kernel(r);  // Compute the kernel function value
//         density += pj->mass * W;

//         double dvx = vx - pj->vx;
//         double dvy = vy - pj->vy;
//         double dvz = vz - pj->vz;

//         double gradW_x, gradW_y, gradW_z;
//         gradKernel(r, dx, dy, dz, &gradW_x, &gradW_y, &gradW_z);  // Compute the gradient of the kernel function

//         density += dt * (dvx * gradW_x + dvy * gradW_y + dvz * gradW_z);
//     }

//     this->density = density;
// }

// void Particle::solvePPE(const std::vector<Particle*>& neighbors, double rho0, double dt, int max_iterations, double tolerance) {
//     int iterations = 0;
//     double p_old = this->pressure;
//     double residual;

//     do {
//         double laplacian_p = 0;

//         for (const Particle* pj : neighbors) {
//             double dx = x - pj->x;
//             double dy = y - pj->y;
//             double dz = z - pj->z;
//             double r = std::sqrt(dx * dx + dy * dy + dz * dz);

//             double gradW_x, gradW_y, gradW_z;
//             gradKernel(r, dx, dy, dz, &gradW_x, &gradW_y, &gradW_z);  // Compute the gradient of the kernel function

//             laplacian_p += pj->mass * (pj->pressure - p_old) * gradW_x / pj->density;
//         }

//         double target_laplacian_p = (rho0 - density) / (dt * dt);
//         double delta_p = (target_laplacian_p - laplacian_p) / (neighbors.size() + 1);

//         p = p_old + delta_p;
//         residual = std::abs(delta_p);

//         iterations++;
//     } while (iterations < max_iterations && residual > tolerance);
// }


// void updateParticles(std::vector<Particle>& particles, double dt, double rho0, double nu, double radius, int max_iterations, double tolerance) {
//     for (Particle& pi : particles) {
//         auto neighbors = pi.findNeighbors(particles, radius);
//         pi.computeForces(neighbors, nu);
//     }

//     for (Particle& pi : particles) {
//         auto neighbors = pi.findNeighbors(particles, radius);
//         pi.computeIntermediateDensity(neighbors, dt);
//         pi.solvePPE(neighbors, rho0, dt, max_iterations, tolerance);
//     }

//     for (Particle& pi : particles) {
//         auto neighbors = pi.findNeighbors(particles, radius);
//         double f_pressure_x = 0, f_pressure_y = 0, f_pressure_z = 0;

//         for (const Particle* pj : neighbors) {
//             double dx = pi.x - pj->x;
//             double dy = pi.y - pj->y;
//             double dz = pi.z - pj->z;
//             double r = std::sqrt(dx * dx + dy * dy + dz * dz);

//             double gradW_x, gradW_y, gradW_z;
//             gradKernel(r, dx, dy, dz, &gradW_x, &gradW_y, &gradW_z);  // Compute the gradient of the kernel function

//             f_pressure_x -= (pi.mass / pi.density) * pi.pressure * gradW_x;
//             f_pressure_y -= (pi.mass / pi.density) * pi.pressure * gradW_y;
//             f_pressure_z -= (pi.mass / pi.density) * pi.pressure * gradW_z;
//         }

//         pi.vx += dt * f_pressure_x / pi.mass;
//         pi.vy += dt * f_pressure_y / pi.mass;
//         pi.vz += dt * f_pressure_z / pi.mass;

//         pi.x += dt * pi.vx;
//         pi.y += dt * pi.vy;
//         pi.z += dt * pi.vz;
//     }
// }

glm::vec<3,double> ACCELERATION_DUE_TO_GRAVITY = glm::vec<3,double>(0.0,-9.8,0.0);
glm::vec<3,double> computeForceFromGravity(Particle& particle) {
    return particle.mass * ACCELERATION_DUE_TO_GRAVITY;
}

glm::vec<3,double> computeForceFromViscosity(Particle& particle, double viscosity, double h) {
    glm::vec<3,double> result = glm::vec<3,double>(0.0);
    glm::vec<3,double> diff;
    for (Particle* neighbor : particle.neighbors) {
        diff = particle.position - neighbor->position;
        result += ( (neighbor->mass / neighbor->density) * (particle.velocity - neighbor->velocity) * 
            (glm::dot(diff,gradKernel(particle.position, neighbor->position, h))/(glm::dot(diff,diff)+0.01*h*h)) );
    }
    return 2.0 * result;
}

std::vector<Particle*> Particle::findNeighbors(const std::vector<Particle>& particles, double radius) {
    std::vector<Particle*> neighbors;
    for (const Particle& pj : particles) {
        if (&pj == this) continue;  // skip
        double dist = glm::distance(this->position, pj.position);
        if (dist <= radius) {
            neighbors.push_back(const_cast<Particle*>(&pj));  // store the pointer to the neighbor particle
        }
    }
    return neighbors;
}

void setNeighbors(std::vector<Particle>& particles) {
    for (Particle& particle : particles) {
        particle.neighbors = particle.findNeighbors(particles, 0.1);
    }
}

void computePartialVelocities(std::vector<Particle>& particles, double dt, double h, double viscosity) {
    for (Particle& particle : particles) {
        glm::vec<3,double> F_visc = computeForceFromViscosity(particle, viscosity, h);
        glm::vec<3,double> F_grav = computeForceFromGravity(particle);
        particle.partialVelocity += dt * (F_visc + F_grav) / particle.mass;
    }
}

void computePartialDensity(Particle& particle, double dt, double h) {
    double baseDensityFromMass = 0;
    double relativeDensityChange = 0;
    for (Particle* neighbor : particle.neighbors) {
        baseDensityFromMass += neighbor->mass * kernel(particle.position, neighbor->position, h);
        relativeDensityChange += glm::dot(particle.velocity - neighbor->velocity, gradKernel(particle.position, neighbor->velocity, h));
    }
    particle.partialDensity = baseDensityFromMass + dt * relativeDensityChange;
}

void computePartialDensities(std::vector<Particle>&  particles, double dt, double h) {
    for (Particle& particle : particles) {
        computePartialDensity(particle, dt, h);
    }
}

void computePressuresFromPoisson(std::vector<Particle>&  particles, double h) {

}

void computePressureForceOnParticle(Particle& particle, double h) {
    glm::vec<3,double> sum = glm::vec<3,double>(0.0);
    for (Particle* neighbor : particle.neighbors) {
        sum += neighbor->mass * (particle.pressure / (particle.density*particle.density) + 
            neighbor->pressure / (neighbor->density*neighbor->density)) * gradKernel(particle.position, neighbor->position, h);
    }
    particle.forcePressure = - (particle.mass / particle.partialDensity) * particle.density * sum;
}

void computePressureForces(std::vector<Particle>&  particles, double h) {
    for (Particle& particle : particles) {
        computePressureForceOnParticle(particle, h);
    }
}

void updateFinalVelocityAndPosition(std::vector<Particle>&  particles, double dt) {
    for (Particle& particle : particles) {
        particle.velocity = particle.partialVelocity + dt * particle.forcePressure / particle.mass;
        particle.position += dt * particle.velocity;
    }
}

void updateParticles(std::vector<Particle>& particles, double dt, double rho0, double viscosity, double radius, int max_iterations, double tolerance, double h) {
    setNeighbors(particles);
    computePartialVelocities(particles, dt, h, viscosity);
    computePartialDensities(particles, dt, h);
    computePressuresFromPoisson(particles, h);
    computePressureForces(particles, h);
    updateFinalVelocityAndPosition(particles, dt);
}






