#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "particle.hpp"
#include "utils/constVector.hpp"


class Scene {
public:
    Scene(float viewWidth, float viewHeight, float kernelRadius) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->kernelRadius = kernelRadius;
    }

    virtual void createScene() = 0;
    virtual void update() = 0;
    ConstVectorWrapper<Particle> getParticles() {
        return ConstVectorWrapper(&particles);
    }
protected:
    std::vector<Particle> particles;
    float viewWidth;
    float viewHeight;
    float kernelRadius;
};