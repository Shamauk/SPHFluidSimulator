#include "sceneManager.hpp"

void SceneManager::changeScene(short sceneNumber) {
    std::cout << "Changing scene to scene #" << sceneNumber << std::endl;
    int index = sceneNumber - 1;
    if (index < 0 || index >= availableScenes.size()) {
        std::cout << "Error: Unkown Scene Selected" << std::endl;
        return;
    }

    this->activeScene = this->availableScenes.at(index);
    this->activeScene->createScene();
}