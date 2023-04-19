#pragma once
#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <iostream>

// Scenes
#include "../scenes/dummyScene.hpp"
#include "../scenes/waterCometScene.hpp"
#include "../scenes/damBreakScene.hpp"
#include "../scenes/rainScene.hpp"
#include "../scenes/fountainScene.hpp"

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
    void setupSceneConfig(SimulatorManager &simulatorManager) {
        activeScene->setupSceneConfig(simulatorManager);
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
    std::vector<Scene *> availableScenes { new WaterCometScene(), new DamBreakScene(), 
        new RainScene(), new FountainScene() };
};

#endif