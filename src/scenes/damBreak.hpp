#pragma once
#include "../scene.hpp"

class DamBreak : Scene {
public:
    DamBreak() : Scene(1200, 900, "Dam Break") {}

    void createScene() override;
    void update(SimulatorManager &) override;
private: 
    const int DAM_PARTICLES = 500;
    int timeStepsToCollapse = 100;
    int counter = 0;
};