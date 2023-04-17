#pragma once

#include "../scene.hpp"

class Bleakfalls : public Scene {
public:
    Bleakfalls(float viewWidth, float viewHeight) : Scene(viewWidth, viewHeight) {}
    void createScene() override;
    void update() override;
private:
    const int DAM_PARTICLES = 500;
};