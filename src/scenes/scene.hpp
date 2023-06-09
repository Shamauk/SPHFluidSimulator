#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "../datastructures/particle.hpp"
#include "../datastructures/parameter.hpp"
#include "../managers/simulatorManager.hpp"
#include "../utils/constVector.hpp"

// Boundaries
#include "../boundaries/positionalBoundary.hpp"
#include "../boundaries/linearBoundary.hpp"
#include "../boundaries/multiBoundary.hpp"
#include "../boundaries/verticalBoundary.hpp"

class Scene {
public:
    Scene(float viewWidth, float viewHeight, std::string name) {
        this->viewWidth = viewWidth;
        this->playWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->playHeight = viewHeight;
        this->name = name;
    }

    virtual void createScene() {
        this->particles = std::vector<Particle>();
    }
    virtual void setupSceneConfig() = 0;
    virtual void update() = 0;
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
    float getPlayHeight() {
        return playHeight;
    }
    float getPlayWidth() {
        return playWidth;
    }

    virtual Boundary *getBoundary() = 0;

protected:
    std::vector<Particle> particles;
    float viewWidth;
    float viewHeight;
    float playWidth;
    float playHeight;
    float particleRadius = 8.f;
    std::string name;
};