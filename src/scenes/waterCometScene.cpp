#include "waterCometScene.hpp"

void WaterCometScene::createScene() {
	Scene::createScene();
	for (float y = viewHeight - particleRadius * 2.f; y > viewHeight - particleRadius - 15.f*2.f*particleRadius; y -= 2.2*particleRadius)
	{
		for (float x = viewWidth - particleRadius; x > viewWidth - particleRadius - 15.f*2.f*particleRadius; x -= 2.2*particleRadius)
		{
			Particle particle(x, y);
			particle.setVelocity(-10000.f, 0.f);
			particles.push_back(particle);
		}
	}
}