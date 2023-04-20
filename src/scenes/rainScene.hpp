#pragma once

#include <cstdlib>

#include "scene.hpp"

class RainScene : public Scene {
public:
    RainScene() : Scene(2400, 1800, "Rain") {
        boundary = new PositionalBoundary();
        boundary->setMinX(0.f);
        boundary->setMaxX(viewWidth);
        boundary->setMinY(0.f);
        boundary->setMaxY(viewHeight);
    }

    void createScene() override {
        Scene::createScene();
    }
    void setupSceneConfig() override {}
    void update() override;

    std::vector<Parameter> getParameters() override {
        return std::vector<Parameter> {
            Parameter{"Inverse Rain Rate", &RAIN_RATE, 1.f, 100.f},
            Parameter{"Density Percentage", &DENSITY_PERCENTAGE, 0.01f, 0.85f},
        };
    }

    Boundary *getBoundary() override { return boundary; }
private:
    float RAIN_RATE = 200.f;
    float DENSITY_PERCENTAGE = .08f;
    const int NUM_PARTICLES_WIDTH = (getViewWidth() / getParticleRadius()) * DENSITY_PERCENTAGE;
    int counter = 0;

    PositionalBoundary *boundary;
};