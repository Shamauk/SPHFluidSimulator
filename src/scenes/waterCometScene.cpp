#include "waterCometScene.hpp"

void WaterCometScene::createScene() {
	Scene::createScene();
	for (float y = viewHeight - particleRadius * 2.f; y > viewHeight - particleRadius - 15.f*2.f*particleRadius; y -= 2.0*particleRadius)
	{
		for (float x = viewWidth - particleRadius; x > viewWidth - particleRadius - 15.f*2.f*particleRadius; x -= 2.0*particleRadius)
		{
			Particle particle(x + 0.25*(rand() % (int)particleRadius), y + 0.25*(rand() % (int)particleRadius));
			particle.setVelocity(-initialVelocity, 0.f);
			particles.push_back(particle);
		}
	}
}