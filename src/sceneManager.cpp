#include "sceneManager.hpp"

void SceneManager::changeScene(short sceneNumber) {
    std::cout << "Changing scene to scene #" << sceneNumber << std::endl;
    switch (sceneNumber) {
        case 1: 
            this->activeScene = new Bleakfalls();
            this->activeScene->createScene();
            break;
        default:
            std::cout << "Unknown scene selected" << std::endl;
    }
}