#pragma once

#include <glm/glm.hpp>
#include <vector>

class Particle {
public:
    Particle(float x, float y) : position(x, y), velocity(0, 0), force(0, 0),
        density(0.0), pressure(0.0) {}

    const glm::vec2 &getPosition() const;
    const glm::vec2 &getVelocity() const;
    const glm::vec2 &getForce() const;
    float getDensity();
    float getPressure();

    void setForce(const glm::vec2 &force);
    void setPosition(const glm::vec2 &position);
    void setVelocity(const glm::vec2 &velocity);
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