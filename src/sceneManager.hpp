#pragma once
#include <iostream>

#include "scene.hpp"

// Scenes
#include "scenes/bleakfalls.hpp"
#include "scenes/dummyScene.hpp"

class SceneManager {
public:
    SceneManager(float viewWidth, float viewHeight) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
    }

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
private:
    float viewWidth;
    float viewHeight;
    Scene *activeScene = new DummyScene();
};