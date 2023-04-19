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
        pi.setDensity(MASS * kernel->poly6Kernel(glm::vec2(0.f)));
		for (auto *pj : pi.getNeigbors())
		{
            pi.setDensity(pi.getDensity() + MASS * 
                kernel->poly6Kernel(pi.getPosition() - pj->getPosition()));
		}
        pi.setPressure(PRESSURE_STIFFNESS * (pi.getDensity() - REST_DENSITY));
	}
}

void MullerSimulator::ComputeForces(ConstVectorWrapper<Particle> particles)
{
	for (auto &pi : particles)
	{
        glm::vec2 fpress(0.f, 0.f);
        glm::vec2 fvisc(0.f, 0.f);
		for (auto *pj : pi.getNeigbors())
		{
            glm::vec2 rij = pi.getPosition() - pj->getPosition();

			fpress += MASS * (pi.getPressure() + pj->getPressure()) 
				/ (2.f * pj->getDensity()) * kernel->gradientSpikyKernel(rij);
			fvisc += VISCOSITY_FACTOR * MASS * (pj->getVelocity() - pi.getVelocity()) / pj->getDensity() 
				* kernel->laplacianViscosityKernel(rij);			
		}
        glm::vec2 fgrav = ACCELERATION_DUE_TO_GRAVITY * MASS / pi.getDensity();
        glm::vec2 totalForce = fpress + fvisc + fgrav;
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