#pragma once

#include "../scene.hpp"

class DummyScene : public Scene {
public:
    DummyScene() : Scene(800,600,1) {};
    void createScene() {};
    void update() {};
};