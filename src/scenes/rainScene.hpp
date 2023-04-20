#pragma once

#include <cstdlib>

#include "scene.hpp"

class RainScene : public Scene {
public:
    RainScene() : Scene(2400, 1800, "Rain") {}
    void createScene() override {
        Scene::createScene();
    }
    void setupSceneConfig(SimulatorManager &simulatorManager) override {}
    void update(SimulatorManager &simulatorManager) override;

    std::vector<Parameter> getParameters() {
        return std::vector<Parameter> {
            Parameter{"Inverse Rain Rate", &RAIN_RATE, 1.f, 100.f},
            Parameter{"Density Percentage", &DENSITY_PERCENTAGE, 0.01f, 0.85f},
        };
    }
private:
    float RAIN_RATE = 200.f;
    float DENSITY_PERCENTAGE = .08f;
    const int NUM_PARTICLES_WIDTH = (getViewWidth() / getParticleRadius()) * DENSITY_PERCENTAGE;
    int counter = 0;
};