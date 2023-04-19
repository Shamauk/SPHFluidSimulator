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
    for (float y = particleRadius; y < this->viewHeight - particleRadius; y += particleRadius * 1.25)
	{
		for (float x = particleRadius; x < this->viewWidth / 4; x += particleRadius * 1.25)
		{
			if (particles.size() < DAM_PARTICLES)
			{
				particles.push_back(Particle(x, y));
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
        collapseRightWall(simulatorManager);
        hasCollapsed = true;
    }
}

void collapseRightWall(SimulatorManager & simulatorManager) {
    simulatorManager.resetBoundary();
}

