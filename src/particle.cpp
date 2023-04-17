#include "particle.hpp"

const glm::vec2 &Particle::getPosition() const {
    return this->position;
}

const glm::vec2 &Particle::getVelocity() const {
    return this->velocity;
}

const glm::vec2 &Particle::getForce() const {
    return this->force;
}

float Particle::getDensity() {
    return this->density;
}

float Particle::getPressure() {
    return this->pressure;
}


void Particle::setForce(const glm::vec2 &force) {
    this->force.x = force.x;
    this->force.y = force.y;
}

void Particle::setPosition(const glm::vec2 &position) {
    this->position.x = position.x;
    this->position.y = position.y;
}

void Particle::setVelocity(const glm::vec2 &velocity) {
    this->velocity.x = velocity.x;
    this->velocity.y = velocity.y;
}

void Particle::setDensity(float density) {
    this->density = density;
}

void Particle::setPressure(float pressure) {
    this->pressure = pressure;
}