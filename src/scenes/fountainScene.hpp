#pragma once

#include "scene.hpp"

class FountainScene : public Scene {
public:
    FountainScene() : Scene(2400, 1800, "Fountain") {
        boundary = new PositionalBoundary();
        boundary->setMinX(0.f);
        boundary->setMaxX(viewWidth);
        boundary->setMinY(0.f);
        boundary->setMaxY(viewHeight);
    }

    void createScene() override {
        Scene::createScene();
    }
    void setupSceneConfig() override {
        FOUNTAIN_RATE_FRAMES = FOUNTAIN_RATE_SECONDS / 1.f;
    }
    void update() override;

    std::vector<Parameter> getParameters() override {
        return std::vector<Parameter> {
            Parameter{"Inverse Fountain Rate", &FOUNTAIN_RATE_FRAMES, 1.f, 100.f},
            Parameter{"Initial Force", &INITIAL_FORCE, 0.01f, 100.f},
        };
    }

    Boundary *getBoundary() { return boundary; }

private:
    const float FOUNTAIN_RATE_SECONDS = 32.f;

    float INITIAL_FORCE = 1500.f;
    float FOUNTAIN_RATE_FRAMES;
    int counter = 0;

    PositionalBoundary *boundary;
};