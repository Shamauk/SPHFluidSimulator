#include "waterfallScene.hpp"

void WaterfallScene::createScene() {
    particles.clear();
    spawnWater();
}

void WaterfallScene::spawnWater() {
    int amtParticlesInX = (int) WATER_SPAWN_AMT / (int) WATER_FALL_HEIGHT;
    for (int i = 0; i < amtParticlesInX; i++) {
        for (int j = 0; j < WATER_FALL_HEIGHT; j++) {
            particles.push_back(Particle(viewWidth + 2*particleRadius + i*2*particleRadius + (rand() % (int) (0.25f * particleRadius)), 
                viewHeight + 2*particleRadius + i * particleRadius + j*2*particleRadius + (rand() % (int) (0.25f * particleRadius))));
        }
    }
}