#include "particle.hpp"

const glm::vec2 &Particle::getPosition() const {
    return this->position;
}

const glm::vec2 &Particle::getVelocity() const {
    return this->velocity;
}

const glm::vec2 &Particle::getIntermVelocity() const {
    return this->intermVelocity;
}

const glm::vec2 &Particle::getForce() const {
    return this->force;
}

float Particle::getDensity() {
    return this->density;
}

float Particle::getIntermDensity() {
    return this->intermDensity;
}

float Particle::getPressure() {
    return this->pressure;
}


void Particle::setForce(float x, float y) {
    this->force.x = x;
    this->force.y = y;
}

void Particle::setPosition(float x, float y) {
    this->position.x = x;
    this->position.y = y;
}

void Particle::setVelocity(float x, float y) {
    this->velocity.x = x;
    this->velocity.y = y;
}

void Particle::setIntermVelocity(float x, float y) {
    this->intermVelocity.x = x;
    this->intermVelocity.y = y;
}

void Particle::setDensity(float density) {
    this->density = density;
}

void Particle::setIntermDensity(float density) {
    this->intermDensity = density;
}

void Particle::setPressure(float pressure) {
    this->pressure = pressure;
}