#pragma once

#include "scene.hpp"

class FountainScene : public Scene {
public:
    FountainScene() : Scene(2400, 1800, "Fountain") {}
    void createScene() override {
        Scene::createScene();
    }
    void setupSceneConfig(SimulatorManager &simulatorManager) override {
        FOUNTAIN_RATE_FRAMES = FOUNTAIN_RATE_SECONDS / simulatorManager.getTimeStep();
    }
    void update(SimulatorManager &simulatorManager) override;

    std::vector<Parameter> getParameters() override {
        return std::vector<Parameter> {
            Parameter{"Inverse Fountain Rate", &FOUNTAIN_RATE_FRAMES, 1.f, 100.f},
            Parameter{"Initial Force", &INITIAL_FORCE, 0.01f, 100.f},
        };
    }

private:
    const float FOUNTAIN_RATE_SECONDS = .05f;

    float INITIAL_FORCE = 1.f;
    float FOUNTAIN_RATE_FRAMES;
    int counter = 0;
};