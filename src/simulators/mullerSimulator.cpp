#include "mullerSimulator.hpp"

void MullerSimulator::update(ConstVectorWrapper<Particle> particles) {
    GetNeighbors(particles);
    ComputeDensityPressure(particles);
	ComputeForces(particles);
	Integrate(particles);
}

void MullerSimulator::GetNeighbors(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
        pi.getNeigbors().clear();
        for (auto &pj : particles) {
            if (&pi == &pj) continue;
            if (solenthalerKernel.isWithinKernelSpace(pi.getPosition() - pj.getPosition())) {
                pi.getNeigbors().push_back(&pj);
            }
        }
    }
}

void MullerSimulator::ComputeDensityPressure(ConstVectorWrapper<Particle> particles)
{
	for (auto &pi : particles)
	{
        // Include yourself in density
        pi.setDensity(MASS * solenthalerKernel.poly6Kernel(glm::vec2(0.f)));
		for (auto *pj : pi.getNeigbors())
		{
            pi.setDensity(pi.getDensity() + MASS * 
                solenthalerKernel.poly6Kernel(pi.getPosition() - pj->getPosition()));
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

			if (solenthalerKernel.isWithinKernelSpace(rij))
			{
				fpress += MASS * (pi.getPressure() + pj->getPressure()) 
                    / (2.f * pj->getDensity()) * solenthalerKernel.gradientSpikyKernel(rij);
				fvisc += VISCOSITY_FACTOR * MASS * (pj->getVelocity() - pi.getVelocity()) / pj->getDensity() 
                    * solenthalerKernel.laplacianViscosityKernel(rij);
			}
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
        boundary->enforceBoundary(particle, kernelRadius);
	}
}