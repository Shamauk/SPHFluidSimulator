#pragma once

#include <glm/glm.hpp>
#include <vector>

class Particle {
public:
    Particle(float x, float y) : position(x, y), velocity(0, 0), force(0, 0),
        density(0.f), pressure(0.f) {}

    Particle(const Particle &particle) : 
        position(particle.position.x, particle.position.y),
        velocity(particle.velocity.x, particle.velocity.y),
        force(particle.force.x, particle.force.y),
        density(particle.density),
        pressure(particle.pressure) {}

    const glm::vec2 &getPosition() const;
    const glm::vec2 &getVelocity() const;
    const glm::vec2 &getForce() const;
    float getDensity();
    float getPressure();

    void setForce(float x, float y);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);
    void setDensity(float density);
    void setPressure(float pressure);

    std::vector<Particle*> &getNeigbors() {
        return neighbors;
    }

private:
    glm::vec2 position, velocity, force;
    float density, pressure;
    std::vector<Particle*> neighbors;

};