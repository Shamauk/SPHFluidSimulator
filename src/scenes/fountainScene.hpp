#pragma once

#include "scene.hpp"

class FountainScene : public Scene {
public:
    FountainScene() : Scene(2400, 1800, "Fountain") {}
    void createScene() override {
        Scene::createScene();
    }
    void setupSceneConfig(SimulatorManager &) override {}
    void update(SimulatorManager &simulatorManager) override;

private:
    const int FOUNTAIN_RATE = 18;
    const float INITIAL_VELOCITY = 18000.f;
    int counter = 0;
};