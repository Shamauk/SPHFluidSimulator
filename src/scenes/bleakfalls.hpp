#pragma once

#include "../scene.hpp"

class Bleakfalls : public Scene {
public:
    Bleakfalls(float viewWidth, float viewHeight, float kernelRadius) : Scene(viewWidth, viewHeight, kernelRadius) {}
    void createScene() override;
    void update() override;
private:
    const int DAM_PARTICLES = 500;
};