#pragma once

#include "scene.hpp"

class DummyScene : public Scene {
public:
    DummyScene() : Scene(800,600,"Dummy Scene") {};
    void createScene() override {};
    void update() override {};
    void setupSceneConfig() override {};
    Boundary *getBoundary() { return nullptr; };
};