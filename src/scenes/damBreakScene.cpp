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

void DamBreakScene::setupSceneConfig(SimulatorManager &simulatorManager) {
    createRightWall(simulatorManager, viewWidth / 4.0);
}

void createRightWall(SimulatorManager &simulatorManager, float xPos) {
    simulatorManager.setBoundaryX(xPos);
}

void DamBreakScene::update(SimulatorManager &simulatorManager) {
    if (!hasCollapsed && ++this->counter > this->TIME_STEP_COLLAPSE) {
		std::cout << "Collapsing the wall" << std::endl;
        collapseRightWall(simulatorManager);
        hasCollapsed = true;
    }
}

void collapseRightWall(SimulatorManager & simulatorManager) {
    simulatorManager.resetBoundary();
}

