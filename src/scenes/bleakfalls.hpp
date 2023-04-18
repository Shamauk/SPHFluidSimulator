#pragma once

#include "../scene.hpp"

class Bleakfalls : public Scene {
public:
    Bleakfalls() : Scene(1200, 900, "Bleakfalls") {}
    void createScene() override;
    void update() override;
private:
    const int DAM_PARTICLES = 500;
};