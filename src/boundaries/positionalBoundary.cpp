#include "positionalBoundary.hpp"

void PositionalBoundary::enforceBoundary(Particle &particle, float epsilon) {
    if (particle.getPosition().x - epsilon < getMinX())
    {
        particle.setVelocity(particle.getVelocity() * glm::vec2(BOUNDARY_DAMPING, 1));
        particle.setPosition(glm::vec2(epsilon, particle.getPosition().y));
    }
    if (particle.getPosition().x + epsilon > getMaxX())
    {
        particle.setVelocity(particle.getVelocity() * glm::vec2(BOUNDARY_DAMPING, 1));
        particle.setPosition(glm::vec2(getMaxX() - epsilon, particle.getPosition().y));
    }
    if (particle.getPosition().y - epsilon < getMinY())
    {
        particle.setVelocity(particle.getVelocity() * glm::vec2(1, BOUNDARY_DAMPING));
        particle.setPosition(glm::vec2(particle.getPosition().x, epsilon));
    }
    if (particle.getPosition().y + epsilon > getMaxY())
    {
        particle.setVelocity(particle.getVelocity() * glm::vec2(1, BOUNDARY_DAMPING));
        particle.setPosition(glm::vec2(particle.getPosition().x, getMaxY() - epsilon));
    }
}