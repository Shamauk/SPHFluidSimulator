#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "particle.hpp"
#include "simulatorManager.hpp"
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
    virtual void update(SimulatorManager &) = 0;
    ConstVectorWrapper<Particle> getParticles() {
        return ConstVectorWrapper(&this->particles);
    }
    float getParticleRadius() {
        return this->particleRadius;
    }
    std::string getName() {
        return this->name;
    }
    float getViewWidth() {
        return viewWidth;
    }
    float getViewHeight() {
        return viewHeight;
    }
protected:
    std::vector<Particle> particles;
    float viewWidth;
    float viewHeight;
    float particleRadius = 8.f;
    std::string name;
};