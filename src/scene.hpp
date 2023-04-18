#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "particle.hpp"
#include "utils/constVector.hpp"


class Scene {
public:
    Scene(float viewWidth, float viewHeight, std::string name) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->name = name;
    }

    virtual void createScene() {
        this->particles = std::vector<Particle>();
    }
    virtual void update() = 0;
    ConstVectorWrapper<Particle> getParticles() {
        return ConstVectorWrapper(&this->particles);
    }
    float getParticleRadius() {
        return this->particleRadius;
    }
    std::string getName() {
        return this->name;
    }
protected:
    std::vector<Particle> particles;
    float viewWidth;
    float viewHeight;
    float particleRadius = 8.f;
    std::string name;
};