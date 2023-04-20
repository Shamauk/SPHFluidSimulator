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
    void update() {
        activeScene->update();
    }
    void setupSceneConfig() {
        activeScene->setupSceneConfig();
    }
    void reset() {
        activeScene->createScene();
    }
    std::vector<Parameter> getParameters() {
        return activeScene->getParameters();
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
    Boundary *getBoundary() {
        return activeScene->getBoundary();
    }
private:
    static SceneManager sceneManager;
    Scene *activeScene = new DummyScene();
    std::vector<Scene *> availableScenes { new WaterCometScene(), new DamBreakScene(), 
        new RainScene(), new FountainScene() };
};

#endif