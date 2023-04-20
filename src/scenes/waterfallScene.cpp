#include "waterfallScene.hpp"

void WaterfallScene::createScene() {
    particles.clear();
    spawnWater();
}

void WaterfallScene::spawnWater() {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 25; j++) {
            particles.push_back(Particle(viewWidth + 2*particleRadius + i*2*particleRadius + (rand() % (int) (0.25f * particleRadius)), 
                viewHeight + 2*particleRadius + i * particleRadius + j*2*particleRadius + (rand() % (int) (0.25f * particleRadius))));
        }
    }
}