#pragma once

#include "../scene.hpp"

class WaterCometScene : public Scene {
public:
    WaterCometScene() : Scene(1200, 900, "Water Comet") {}
    void createScene() override;
    void update(SimulatorManager &) {}
    void setupSceneConfig(SimulatorManager &simulatorManager) {};
private:
    const int DAM_PARTICLES = 500;
};