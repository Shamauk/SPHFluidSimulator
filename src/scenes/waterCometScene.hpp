#pragma once

#include "scene.hpp"

class WaterCometScene : public Scene {
public:
    WaterCometScene() : Scene(1200, 800, "Water Comet") {
        boundary = new PositionalBoundary();
        boundary->setMinX(0.f);
        boundary->setMaxX(viewWidth);
        boundary->setMinY(0.f);
        boundary->setMaxY(viewHeight);
    }
    void createScene() override;
    void update() override {}
    void setupSceneConfig() override {};
    Boundary *getBoundary() override { return boundary; }

    std::vector<Parameter> getParameters() override {
        return {
            Parameter{"Initial Velocity", &initialVelocity, 0.f, 1000.f},
        };
    }

private:
    float initialVelocity = 100.f;

    PositionalBoundary *boundary;
};