#include "sceneManager.hpp"

void SceneManager::changeScene(short sceneNumber) {
    switch (sceneNumber) {
        case 0: 
            this->activeScene = new Bleakfalls(this->viewWidth, this->viewHeight);
            this->activeScene->createScene();
            break;
        default:
            std::cout << "Unknown scene selected" << std::endl;
    }
}