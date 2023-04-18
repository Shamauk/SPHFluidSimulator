#include "damBreak.hpp"

void collapseRightWall(SimulatorManager &);
void createRightWall(SimulatorManager &);

void DamBreak::createScene() {

}

void createRightWall(SimulatorManager &) {

}

void DamBreak::update(SimulatorManager &simulatorManager) {
    if (this->counter++ % this->timeStepsToCollapse == 0) {
        this->timeStepsToCollapse = -1;
        collapseRightWall(simulatorManager);
    }
}

void collapseRightWall(SimulatorManager & simulatorManager) {
    simulatorManager.resetBoundary();
}

