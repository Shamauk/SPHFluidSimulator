#pragma once

#include <cstdlib>

#include "../scene.hpp"

class RainScene : public Scene {
public:
    RainScene() : Scene(2400, 1800, "Rain") {}
    void createScene() {
        Scene::createScene();
    }
    void setupSceneConfig(SimulatorManager &) {}
    void update(SimulatorManager &simulatorManager);
private:
    const int RAIN_RATE = 52;
    const float DENSITY_PERCENTAGE = .08f;
    const int NUM_PARTICLES_WIDTH = (getViewWidth() / getParticleRadius()) * DENSITY_PERCENTAGE;
    int counter = 0;
};