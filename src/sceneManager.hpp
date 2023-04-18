#pragma once
#include <iostream>

#include "scene.hpp"

// Scenes
#include "scenes/bleakfalls.hpp"
#include "scenes/dummyScene.hpp"

class SceneManager {
public:
    ConstVectorWrapper<Particle> getParticles() {
        return this->activeScene->getParticles();
    }
    void changeScene(short sceneNumber);
    float getParticleRadius() {
        return activeScene->getParticleRadius();
    }
    void update() {
        activeScene->update();
    }
    void reset() {
        activeScene->createScene();
    }
    const std::string getName() {
        return activeScene->getName();
    }
    float getViewWidth() {
        return activeScene->getViewWidth();
    }
    float getViewHeight() {
        return activeScene->getViewHeight();
    }
private:
    Scene *activeScene = new DummyScene();
};