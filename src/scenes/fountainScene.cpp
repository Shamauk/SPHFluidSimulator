#include "fountainScene.hpp"

void FountainScene::update() {
    if (++counter > FOUNTAIN_RATE_FRAMES) {
        counter = 0;
        Particle particle(viewWidth * 0.5 + rand() % 10, viewHeight * 0.3);
        particle.setVelocity(0.f, INITIAL_FORCE);
        particles.push_back(particle);
    }
}