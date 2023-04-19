#include "rainScene.hpp"

void RainScene::update(SimulatorManager &simulatorManager) {
    if (++counter > RAIN_RATE) {
        counter = 0;
        for (int i = 0; i < NUM_PARTICLES_WIDTH; i++) {
            particles.push_back(Particle(rand() % (int) viewWidth, viewHeight - particleRadius));
        }
    }
}