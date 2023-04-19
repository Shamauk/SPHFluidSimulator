#include "simulatorManager.hpp"

void SimulatorManager::changeSimulator(short simulationNumber, float viewWidth, float viewHeight, float particleRadius) {
    std::cout << "Changing simulators to simulator #" << simulationNumber << std::endl;
    switch (simulationNumber) {
        case 1:
            this->activeSimulator = new MullerSimulator(viewWidth, viewHeight, particleRadius);
            break;
        default:
            std::cout << "Cannot find designated simulator" << std::endl;
    };
}