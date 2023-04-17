#include "bleakfalls.hpp"

void Bleakfalls::createScene() {
	for (float y = 2*particleRadius; y < this->viewHeight - 2*particleRadius * 2.f; y += 2*particleRadius)
	{
		for (float x = this->viewWidth / 4; x <= this->viewWidth / 2; x += 2*particleRadius)
		{
			if (particles.size() < DAM_PARTICLES)
			{
				float jitter = static_cast<float>(arc4random()) / static_cast<float>(RAND_MAX);
				particles.push_back(Particle(x + jitter, y));
			}
			else
			{
				return;
			}
		}
	}
}

void Bleakfalls::update() {

}