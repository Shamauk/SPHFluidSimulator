#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "particle.hpp"
#include "utils/constVector.hpp"


class Scene {
public:
    Scene(float viewWidth, float viewHeight) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
    }

    virtual void createScene() {
        particles = std::vector<Particle>();
    }
    virtual void update() = 0;
    ConstVectorWrapper<Particle> getParticles() {
        return ConstVectorWrapper(&particles);
    }
    float getParticleRadius() {
        return particleRadius;
    }
protected:
    std::vector<Particle> particles;
    float viewWidth;
    float viewHeight;
    float particleRadius = 8.f;
};