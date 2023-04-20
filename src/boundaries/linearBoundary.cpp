#include "linearBoundary.hpp"

void LinearBoundary::enforceBoundary(Particle &particle, float epsilon) {
    float x = particle.getPosition().x;

    if (x < xLowRange || x > xHighRange)
        return;

    float y = particle.getPosition().y;

    float lineY = slope * x + yIntercept;

    bool outOfBoundary = false;
    // Check if in critical region
    if (y <= lineY + epsilon && y >= lineY - epsilon) {
        if (outOfBoundsAbove && y > lineY)
            outOfBoundary = true;
        else if (!outOfBoundsAbove && y < lineY)
            outOfBoundary = true;
    }

    if (outOfBoundary) {
        // Calculate the normal vector of the line
        glm::vec2 normalVector(-slope, 1.f);
        normalVector = glm::normalize(normalVector);

        if (!outOfBoundsAbove) {
            normalVector *= -1;
        }

        // Project particle velocity on the normal vector
        glm::vec2 velocityProjection = glm::dot(particle.getVelocity(), normalVector) * normalVector;

        // Update particle velocity and position
        glm::vec2 newVelocity = particle.getVelocity() - velocityProjection;
        particle.setVelocity(newVelocity.x, newVelocity.y);

        // Move the particle to the boundary line
        particle.setPosition(x, lineY);
    }
}