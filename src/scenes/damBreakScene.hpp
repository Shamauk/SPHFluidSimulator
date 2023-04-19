#pragma once
#include "../scene.hpp"

class DamBreakScene : public Scene {
public:
    DamBreakScene() : Scene(1200, 900, "Dam Break") {}

    void createScene() override;
    void update(SimulatorManager &);
    void setupSceneConfig(SimulatorManager &);
private: 
    const int DAM_PARTICLES = 1000;
    const int TIME_STEP_COLLAPSE = 800;
    int counter = 0;
    int hasCollapsed = false;
    
    void createParticleDam();
};