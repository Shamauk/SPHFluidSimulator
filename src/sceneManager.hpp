#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

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
    void update(SimulatorManager &simulatorManager) {
        activeScene->update(simulatorManager);
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
    static SceneManager sceneManager;
    Scene *activeScene = new DummyScene();
};

#endif