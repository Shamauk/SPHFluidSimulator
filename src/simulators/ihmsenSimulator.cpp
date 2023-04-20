#include "ihmsenSimulator.hpp"

void IhmsenSimulator::update(ConstVectorWrapper<Particle> particles) {
    discretization->getNeighbors(particles);
    getDensities(particles);
    computeForces(particles);
    integrate(particles);
}

void IhmsenSimulator::getDensities(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
		float density = kernel->kernel(glm::vec2(0.f));
		for (auto *pj : pi.getNeigbors())
		{
			density += kernel->kernel(pi.getPosition() - pj->getPosition());
		}
		pi.setDensity(MASS * density);

        float tmp = pi.getDensity() / REST_DENSITY;
        pi.setPressure(PRESSURE_STIFFNESS * tmp*tmp*tmp*tmp*tmp*tmp*tmp - 1);
	}
}

void IhmsenSimulator::computeForces(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
        glm::vec2 forceViscosity(0.f);
        glm::vec2 forcePressure (0.f);
        for (auto *pj : pi.getNeigbors()) {
            glm::vec2 rij = pi.getPosition() - pj->getPosition();
            forcePressure += ((pi.getPressure() / (pi.getDensity()*pi.getDensity())) + 
                (pj->getPressure() / (pj->getDensity()*pj->getDensity()))) * kernel->gradientKernel(rij);
            forceViscosity += ((pi.getVelocity() - pj->getVelocity()) / pj->getDensity()) * 
                ((glm::dot(rij, kernel->gradientKernel(rij))) / (glm::dot(rij, rij) + 0.0025f*kernel->getKernelRange()*kernel->getKernelRange()));
        }
        forcePressure *= -1 * MASS * MASS;
        forceViscosity *= 2 * MASS * MASS * VISCOSITY_FACTOR;
        glm::vec2 forceGravity = ACCELERATION_DUE_TO_GRAVITY * MASS;
        glm::vec2 totalForce = forcePressure + forceViscosity + forceGravity;
        pi.setForce(totalForce.x, totalForce.y);
    }
}

void IhmsenSimulator::integrate(ConstVectorWrapper<Particle> particles) {
    for (auto &p : particles) {
        glm::vec2 newVelocity = p.getVelocity() + DT * (p.getForce() / MASS);
        p.setVelocity(newVelocity.x, newVelocity.y);
        glm::vec2 newPosition = p.getPosition() + DT * p.getVelocity();
        p.setPosition(newPosition.x, newPosition.y);
        boundary->enforceBoundary(p, kernel->getKernelRange());
    }
}