#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "../datastructures/particle.hpp"
#include "../datastructures/parameter.hpp"
#include "../managers/simulatorManager.hpp"
#include "../utils/constVector.hpp"

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
    virtual void setupSceneConfig(SimulatorManager &) = 0;
    virtual void update(SimulatorManager &) = 0;
    virtual std::vector<Parameter> getParameters() {
        return std::vector<Parameter>();
    }

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