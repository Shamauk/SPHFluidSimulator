#include "simulatorManager.hpp"

void SimulatorManager::changeSimulator(short simulationNumber, float particleRadius) {
    switch (simulationNumber) {
        case 0:
            this->activeSimulator = new MullerSimulator(this->viewWidth, this->viewHeight, particleRadius);
            break;
        default:
            std::cout << "Cannot find designated simulator" << std::endl;
    };
}