#include "mullerSimulator.hpp"

void MullerSimulator::update(ConstVectorWrapper<Particle> particles) {
    discretization->getNeighbors(particles);
    ComputeDensityPressure(particles);
	ComputeForces(particles);
	Integrate(particles);
}

void MullerSimulator::ComputeDensityPressure(ConstVectorWrapper<Particle> particles)
{
	for (auto &pi : particles)
	{
        // Include yourself in density
		float density = kernel->poly6Kernel(glm::vec2(0.f));
		for (auto *pj : pi.getNeigbors())
		{
			density += kernel->poly6Kernel(pi.getPosition() - pj->getPosition());
		}
		pi.setDensity(MASS * density);
		pi.setDensityError(abs(pi.getDensity() - REST_DENSITY) / REST_DENSITY);
        pi.setPressure(PRESSURE_STIFFNESS * (pi.getDensity() - REST_DENSITY));
	}
}

void MullerSimulator::ComputeForces(ConstVectorWrapper<Particle> particles)
{
	for (auto &pi : particles)
	{
        glm::vec2 forcePressure(0.f, 0.f);
        glm::vec2 forceViscosity(0.f, 0.f);
		for (auto *pj : pi.getNeigbors())
		{
            glm::vec2 rij = pi.getPosition() - pj->getPosition();

			forcePressure += (pi.getPressure() + pj->getPressure()) 
				/ (2.f * pj->getDensity()) * kernel->gradientSpikyKernel(rij);
			forceViscosity += ((pj->getVelocity() - pi.getVelocity()) / pj->getDensity())
				* kernel->laplacianViscosityKernel(rij);			
		}
		forcePressure *= -1 * MASS;
		forceViscosity *= VISCOSITY_FACTOR * MASS;
        glm::vec2 forceGravity = ACCELERATION_DUE_TO_GRAVITY * MASS;
        glm::vec2 totalForce = forcePressure + forceViscosity + forceGravity;
        pi.setForce(totalForce.x, totalForce.y);
	}
}

void MullerSimulator::Integrate(ConstVectorWrapper<Particle> particles)
{
	for (auto &particle : particles)
	{
        glm::vec2 newVelocity = particle.getVelocity() + DT * particle.getForce() / particle.getDensity();
        particle.setVelocity(newVelocity.x, newVelocity.y);
        glm::vec2 newPosition = particle.getPosition() + DT * particle.getVelocity();
        particle.setPosition(newPosition.x, newPosition.y);
        boundary->enforceBoundary(particle, kernel->getKernelRange());
	}
}