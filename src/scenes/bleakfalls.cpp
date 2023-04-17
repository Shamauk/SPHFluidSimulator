#include "bleakfalls.hpp"

void Bleakfalls::createScene() {
	for (float y = this->kernelRadius; y < this->viewHeight - this->kernelRadius * 2.f; y += this->kernelRadius)
	{
		for (float x = this->viewWidth / 4; x <= this->viewWidth / 2; x += this->kernelRadius)
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