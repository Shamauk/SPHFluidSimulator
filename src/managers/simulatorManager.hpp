#pragma once

#include <iostream>

#include "../utils/constVector.hpp"

// Simulators
#include "../simulators/mullerSimulator.hpp"

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

    float getKernelRange() {
        return activeSimulator->getKernelRange();
    }

    short getID() {
        return activeSimulator->getID();
    }

    void setDiscretization(Discretization *discretization) {
        activeSimulator->setDiscretization(discretization);
    }

    size_t getDiscretizationMemoryUsage() {
        return activeSimulator->getDiscretizationMemoryUsage();
    }

    std::string getDiscretizationName() {
        return activeSimulator->getDiscretizationName();
    }

    void resetBoundary() {
        return activeSimulator->resetBoundary();
    }

    void setBoundaryX(float x) {
        activeSimulator->setBoundaryX(x);
    }

    void setBoundaryY(float y) {
        activeSimulator->setBoundaryY(y);
    }

private:
    Simulator *activeSimulator;
};