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
        rightUpwardBoundary = new VerticalBoundary(getViewWidth(), 0.f, 1590, true);
        boundary->addBoundary(rightUpwardBoundary);
    }

    void createScene() override;
    void setupSceneConfig() override {}
    void update() override {}

    std::vector<Parameter> getParameters() override {
        return {
            Parameter{"Waterfall Particles", &WATER_SPAWN_AMT, 1.f, 10000.f},
            Parameter{"Waterfall Height", &WATER_FALL_HEIGHT, 1.f, 100.f},
        };
    }

    Boundary *getBoundary() override {
        return boundary;
    }

    void spawnWater();

private:
    float WATER_SPAWN_AMT = 1250.f;
    float WATER_FALL_HEIGHT = 35.f;

    PositionalBoundary *positionalBoundary;
    LinearBoundary *rampBoundary;
    VerticalBoundary *rightUpwardBoundary;
    MultiBoundary *boundary;
};