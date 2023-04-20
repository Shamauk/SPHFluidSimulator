#include "damBreakScene.hpp"

void collapseRightWall(SimulatorManager &);
void createRightWall(SimulatorManager &, float);

void DamBreakScene::createScene() {
    Scene::createScene();
    createParticleDam();
    counter = 0;
    hasCollapsed = false;
}

void DamBreakScene::createParticleDam() {
    for (float y = particleRadius; y < this->viewHeight - particleRadius; y += particleRadius * 2.0)
	{
		for (float x = particleRadius; x < this->viewWidth / 4; x += particleRadius * 2.0)
		{
			if (particles.size() < DAM_PARTICLES)
			{
				particles.push_back(Particle(x + (rand() % (int)(particleRadius)), y));
			}
			else
			{
				return;
			}
		}
	}
}

void DamBreakScene::setupSceneConfig() {
    createRightWall(viewWidth / 4.0f);
}

void DamBreakScene::createRightWall(float xPos) {
	boundary->setMaxX(xPos);
}

void DamBreakScene::update() {
    if (!hasCollapsed && ++this->counter > this->TIME_STEP_COLLAPSE) {
		std::cout << "Collapsing the wall" << std::endl;
        collapseRightWall();
        hasCollapsed = true;
    }
}

void DamBreakScene::collapseRightWall() {
	boundary->setMaxX(viewWidth);
}

