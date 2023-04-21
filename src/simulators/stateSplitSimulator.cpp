#include "stateSplitSimulator.hpp"

void StateSplitSimulator::update(ConstVectorWrapper<Particle> particles) {
    discretization->getNeighbors(particles);
    getDensities(particles);
    computeIntermediateForcesAndVelocities(particles);
    computeForceExertedByPressure(particles);
    integrate(particles);
}

void StateSplitSimulator::getDensities(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
		float density = kernel->kernel(glm::vec2(0.f));
		for (auto *pj : pi.getNeigbors())
		{
			density += kernel->kernel(pi.getPosition() - pj->getPosition());
		}
		pi.setDensity(MASS * density);
        pi.setDensityError(abs(pi.getDensity() - REST_DENSITY) / REST_DENSITY);
	}
}

void StateSplitSimulator::computeIntermediateForcesAndVelocities(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
        glm::vec2 forceViscosity(0.f);
        for (auto *pj : pi.getNeigbors()) {
            glm::vec2 rij = pi.getPosition() - pj->getPosition();
            forceViscosity += ((pi.getVelocity() - pj->getVelocity()) / pj->getDensity()) * 
                ((glm::dot(rij, kernel->gradientKernel(rij))) / (glm::dot(rij, rij) + 0.0025f*kernel->getKernelRange()*kernel->getKernelRange()));
        }
        forceViscosity *= 2 * MASS * MASS * VISCOSITY_FACTOR;
        glm::vec2 forceGravity = ACCELERATION_DUE_TO_GRAVITY * MASS;
        glm::vec2 totalForce = forceViscosity + forceGravity;

        glm::vec2 intermVelocity = pi.getVelocity() + DT * totalForce / MASS;
        pi.setIntermVelocity(intermVelocity.x, intermVelocity.y);
    }
}

void StateSplitSimulator::computeForceExertedByPressure(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
        float velocityDensity = 0;
        for (auto *pj : pi.getNeigbors()) {
            velocityDensity += glm::dot(pi.getIntermVelocity() - pj->getIntermVelocity(), 
                kernel->gradientKernel(pi.getPosition() - pj->getPosition()));
        }
        pi.setIntermDensity(pi.getDensity() + DT * velocityDensity);
        float tmp = pi.getIntermDensity() / REST_DENSITY;
        pi.setPressure(PRESSURE_STIFFNESS * tmp*tmp*tmp*tmp*tmp*tmp*tmp - 1);
    }

    for (auto &pi : particles) {
        glm::vec2 forcePressure(0.f);
        for (auto *pj : pi.getNeigbors()) {
            glm::vec2 rij = pi.getPosition() - pj->getPosition();
            forcePressure += ((pi.getPressure() / (pi.getIntermDensity()*pi.getIntermDensity())) + 
                (pj->getPressure() / (pj->getIntermDensity()*pj->getIntermDensity()))) * kernel->gradientKernel(rij);
        }
        forcePressure *= -1 * MASS * MASS;
        pi.setForce(forcePressure.x, forcePressure.y);
    }
}

void StateSplitSimulator::integrate(ConstVectorWrapper<Particle> particles) {
    for (auto &p : particles) {
        glm::vec2 newVelocity = p.getIntermVelocity() + DT * (p.getForce() / MASS);
        p.setVelocity(newVelocity.x, newVelocity.y);
        glm::vec2 newPosition = p.getPosition() + DT * p.getVelocity();
        p.setPosition(newPosition.x, newPosition.y);
        boundary->enforceBoundary(p, kernel->getKernelRange());
    }
}