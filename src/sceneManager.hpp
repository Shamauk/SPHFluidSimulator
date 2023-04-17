#pragma once
#include <iostream>
#include "scene.hpp"

// Scenes
#include "scenes/bleakfalls.hpp"
#include "scenes/dummyScene.hpp"

class SceneManager {
public:
    SceneManager(float viewWidth, float viewHeight, float kernelRadius) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->kernelRadius = kernelRadius;
    }

    ConstVectorWrapper<Particle> getParticles() {
        return this->activeScene->getParticles();
    }
    void changeScene(short sceneNumber);
private:
    float viewWidth;
    float viewHeight;
    float kernelRadius;
    Scene *activeScene = new DummyScene();
};