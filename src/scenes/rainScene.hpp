#pragma once

#include <cstdlib>

#include "scene.hpp"

class RainScene : public Scene {
public:
    RainScene() : Scene(2400, 1800, "Rain") {}
    void createScene() override {
        Scene::createScene();
    }
    void setupSceneConfig(SimulatorManager &) override {}
    void update(SimulatorManager &simulatorManager) override;
private:
    const int RAIN_RATE = 52;
    const float DENSITY_PERCENTAGE = .08f;
    const int NUM_PARTICLES_WIDTH = (getViewWidth() / getParticleRadius()) * DENSITY_PERCENTAGE;
    int counter = 0;
};