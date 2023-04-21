#pragma once

#include <glm/glm.hpp>
#include <vector>

class Particle {
public:
    Particle(float x, float y) : position(x, y), velocity(0, 0), intermVelocity(0, 0), force(0, 0),
        density(0.f), pressure(0.f) {}

    Particle(const Particle &particle) : 
        position(particle.position.x, particle.position.y),
        velocity(particle.velocity.x, particle.velocity.y),
        intermVelocity(particle.intermVelocity.x, particle.intermVelocity.y),
        force(particle.force.x, particle.force.y),
        density(particle.density),
        pressure(particle.pressure) {}

    const glm::vec2 &getPosition() const;
    const glm::vec2 &getVelocity() const;
    const glm::vec2 &getIntermVelocity() const;
    const glm::vec2 &getForce() const;
    float getDensity() const;
    float getDensityError() const;
    float getIntermDensity() const;
    float getPressure() const;

    void setForce(float x, float y);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);
    void setIntermVelocity(float x, float y);
    void setDensity(float density);
    void setDensityError(float densityError);
    void setIntermDensity(float density);
    void setPressure(float pressure);

    std::vector<Particle*> &getNeigbors() {
        return neighbors;
    }

private:
    glm::vec2 position, velocity, intermVelocity, force;
    float density, densityError, intermDensity, pressure;
    std::vector<Particle*> neighbors;

};