#ifndef LIGHT_H
#define LIGHT_H

#include "glm/glm.hpp"

struct Light {
    glm::vec3 color;
    glm::vec3 position;
    float power;
};

#endif