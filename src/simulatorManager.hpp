#pragma once

#include <iostream>

#include "simulator.hpp"
#include "utils/constVector.hpp"

// Simulators
#include "simulators/mullerSimulator.hpp"

class SimulatorManager {
public:
    void update(ConstVectorWrapper<Particle> particles) {
        activeSimulator->update(particles);
    }

    const std::string getName() {
        return activeSimulator->getName();
    }

    void changeSimulator(short, float, float, float);

    std::vector<Parameter> getParameters() {
        return activeSimulator->getParameters();
    }

    float getKernelRadius() {
        return activeSimulator->getKernelRadius();
    }

    short getID() {
        return activeSimulator->getID();
    }

    void resetBoundary() {
        return activeSimulator->resetBoundary();
    }

private:
    Simulator *activeSimulator;
    float viewWidth;
    float viewHeight;
};