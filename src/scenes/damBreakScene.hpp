#pragma once
#include "scene.hpp"

class DamBreakScene : public Scene {
public:
    DamBreakScene() : Scene(1200, 900, "Dam Break") {
        boundary = new PositionalBoundary();
        boundary->setMinX(0.f);
        boundary->setMaxX(viewWidth);
        boundary->setMinY(0.f);
        boundary->setMaxY(viewHeight);
    }

    void createScene() override;
    void update() override;
    void setupSceneConfig() override;

    Boundary *getBoundary() override { return boundary; }
private: 
    const int DAM_PARTICLES = 1000;
    const int TIME_STEP_COLLAPSE = 800;
    int counter = 0;
    int hasCollapsed = false;

    PositionalBoundary *boundary;
    
    void createParticleDam();
    void createRightWall(float);
    void collapseRightWall();
};