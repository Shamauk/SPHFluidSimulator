#pragma once

#include "scene.hpp"

class WaterfallScene : public Scene {
public:
    WaterfallScene() : Scene(2400, 1800, "Waterfall") {
        playWidth = 2 * viewWidth;
        playHeight = 2 * viewHeight;
        boundary = new MultiBoundary();
        positionalBoundary = new PositionalBoundary();
        positionalBoundary->setMinX(0.f);
        positionalBoundary->setMaxX(playWidth);
        positionalBoundary->setMinY(0.f);
        positionalBoundary->setMaxY(playHeight);
        boundary->addBoundary(positionalBoundary);
        rampBoundary = new LinearBoundary(0.1f, 1370.f, false, getViewWidth() - 20 * 2.f * particleRadius, getViewWidth() * 2.f);
        boundary->addBoundary(rampBoundary);
        // rightUpwardBoundary = new LinearBoundary(16.1f, -37030.f, false, 2300.f, viewWidth);
        // boundary->addBoundary(rightUpwardBoundary);
        rightUpwardBoundary = new VerticalBoundary(getViewWidth(), 0.f, 1590, true);
        boundary->addBoundary(rightUpwardBoundary);
    }

    void createScene() override;
    void setupSceneConfig() override {}
    void update() override {}

    std::vector<Parameter> getParameters() override {
        return {
            Parameter{"Inverse Water Rate", &WATER_SPAWN_RATE, 1.f, 1000.f},
        };
    }

    Boundary *getBoundary() override {
        return boundary;
    }

    void spawnWater();

private:
    int counter = 0;
    float WATER_SPAWN_RATE = 850.f;

    PositionalBoundary *positionalBoundary;
    LinearBoundary *rampBoundary;
    VerticalBoundary *rightUpwardBoundary;
    MultiBoundary *boundary;
};