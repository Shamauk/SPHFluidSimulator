#pragma once

#include "../scene.hpp"

class DummyScene : public Scene {
public:
    DummyScene() : Scene(800,600,"Dummy Scene") {};
    void createScene() {};
    void update(SimulatorManager &) {};
    void setupSceneConfig(SimulatorManager &) {};
};