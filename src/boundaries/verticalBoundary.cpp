#include "verticalBoundary.hpp"

void VerticalBoundary::enforceBoundary(Particle &particle, float epsilon) {
    if (particle.getPosition().y < yLowRange || particle.getPosition().y > yHighRange)
        return;

    if (outOfBoundsOnRight && particle.getPosition().x + epsilon > X) {
        particle.setPosition(X - epsilon, particle.getPosition().y);
        particle.setVelocity(DAMPING * particle.getVelocity().x, particle.getVelocity().y);
    } else if (!outOfBoundsOnRight && particle.getPosition().x - epsilon < X) {
        particle.setPosition(X + epsilon, particle.getPosition().y);
        particle.setVelocity(DAMPING * particle.getVelocity().x, particle.getVelocity().y);
    }
}