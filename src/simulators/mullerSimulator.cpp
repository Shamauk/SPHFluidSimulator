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
        pi.setForce(fpress + fvisc + fgrav);
	}
}

void MullerSimulator::Integrate(ConstVectorWrapper<Particle> particles)
{
	for (auto &p : particles)
	{
        p.setVelocity(p.getVelocity() + DT * p.getForce() / p.getDensity());
        p.setPosition(p.getPosition() + DT * p.getVelocity());

        // Handle boundary
        if (p.getPosition().x - epsilon < 0.f)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(BOUND_DAMPING, 1));
            p.setPosition(glm::vec2(epsilon, p.getPosition().y));
		}
		if (p.getPosition().x + epsilon > viewWidth)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(BOUND_DAMPING, 1));
            p.setPosition(glm::vec2(viewWidth - epsilon, p.getPosition().y));
		}
		if (p.getPosition().y - epsilon < 0.f)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(1, BOUND_DAMPING));
            p.setPosition(glm::vec2(p.getPosition().x, epsilon));
		}
		if (p.getPosition().y + epsilon > viewHeight)
		{
            p.setVelocity(p.getVelocity() * glm::vec2(1, BOUND_DAMPING));
            p.setPosition(glm::vec2(p.getPosition().x, viewHeight - epsilon));
		}
	}
}