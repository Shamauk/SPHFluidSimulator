#include "mullerSimulator.hpp"

void MullerSimulator::update(ConstVectorWrapper<Particle> particles) {
    ComputeDensityPressure(particles);
	ComputeForces(particles);
	Integrate(particles);
}

void MullerSimulator::Integrate(ConstVectorWrapper<Particle> particles)
{
	for (auto &p : particles)
	{
		// forward Euler integration
        p.setVelocity(p.getVelocity() + DT * p.getForce() / p.getDensity());
        p.setPosition(p.getPosition() + DT * p.getVelocity());

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

void MullerSimulator::ComputeDensityPressure(ConstVectorWrapper<Particle> particles)
{
	for (auto &pi : particles)
	{
        pi.setDensity(0.f);
		for (auto &pj : particles)
		{
            glm::vec2 rij = pi.getPosition() - pj.getPosition();
            float r = glm::length(rij);

			if (r < kernelRadius)
			{
                pi.setDensity(pi.getDensity() + MASS * solenthalerKernel.poly6Kernel(rij));
			}
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
		for (auto &pj : particles)
		{
			if (&pi == &pj)
			{
				continue;
			}

            glm::vec2 rij = pi.getPosition() - pj.getPosition();
            float r = glm::length(rij);

			if (r < kernelRadius)
			{
				fpress += MASS * (pi.getPressure() + pj.getPressure()) 
                    / (2.f * pj.getDensity()) * solenthalerKernel.gradientSpikyKernel(rij);
				fvisc += VISCOSITY_FACTOR * MASS * (pj.getVelocity() - pi.getVelocity()) / pj.getDensity() 
                    * solenthalerKernel.laplacianViscosityKernel(rij);
			}
		}
        glm::vec2 fgrav = ACCELERATION_DUE_TO_GRAVITY * MASS / pi.getDensity();
        pi.setForce(fpress + fvisc + fgrav);
	}
}