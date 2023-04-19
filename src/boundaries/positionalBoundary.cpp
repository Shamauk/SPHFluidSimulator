#include "positionalBoundary.hpp"

void PositionalBoundary::enforceBoundary(Particle &particle, float epsilon) {
    if (particle.getPosition().x - epsilon < minX)
    {
        particle.setVelocity(particle.getVelocity().x * BOUNDARY_DAMPING, particle.getVelocity().y);
        particle.setPosition(minX + epsilon, particle.getPosition().y);
    }
    if (particle.getPosition().x + epsilon > maxX)
    {
        particle.setVelocity(particle.getVelocity().x * BOUNDARY_DAMPING, particle.getVelocity().y);
        particle.setPosition(maxX - epsilon, particle.getPosition().y);
    }
    if (particle.getPosition().y - epsilon < minY)
    {
        particle.setVelocity(particle.getVelocity().x, particle.getVelocity().y * BOUNDARY_DAMPING);
        particle.setPosition(particle.getPosition().x, minY + epsilon);
    }
    if (particle.getPosition().y + epsilon > maxY)
    {
        particle.setVelocity(particle.getVelocity().x, particle.getVelocity().y * BOUNDARY_DAMPING);
        particle.setPosition(particle.getPosition().x, maxY - epsilon);
    }
}