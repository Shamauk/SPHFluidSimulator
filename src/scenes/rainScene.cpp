#include "rainScene.hpp"

void RainScene::update(SimulatorManager &simulatorManager) {
    if (++counter > RAIN_RATE) {
        counter = 0;
        std::vector<Particle *> particleRound;
        for (int i = 0; i < NUM_PARTICLES_WIDTH; i++) {
            while (true) {
                int x = rand() % (int) viewWidth;
                bool isEffected = false;
                for (auto *p : particleRound) {
                    if (p->getPosition().x - x < 2 * particleRadius && x - p->getPosition().x < 2 * particleRadius) {
                        isEffected = true;
                        break;
                    }
                }
                if (isEffected)
                    continue;
                particleRound.push_back(new Particle(x, viewHeight - particleRadius - (rand() % (int) particleRadius)));
                break;
            }
        }
        for (auto *p : particleRound) {
            particles.push_back(*p);
        }
    }
}