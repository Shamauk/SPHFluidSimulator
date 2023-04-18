#pragma once

#include <iostream>

#include "simulator.hpp"
#include "utils/constVector.hpp"

// Simulators
#include "simulators/mullerSimulator.hpp"

class SimulatorManager {
public:
    SimulatorManager(float viewWidth, float viewHeight) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
    }

    void update(ConstVectorWrapper<Particle> particles) {
        activeSimulator->update(particles);
    }

    const std::string getName() {
        return activeSimulator->getName();
    }

    void changeSimulator(short, float);

    std::vector<Parameter> getParameters() {
        return activeSimulator->getParameters();
    }

private:
    Simulator *activeSimulator;
    float viewWidth;
    float viewHeight;
};