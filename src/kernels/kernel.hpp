#pragma once

#include <glm/glm.hpp>
#include <cmath>

class Kernel {
public:
    virtual float getKernelRange() = 0;
    virtual float *getKernelParameterReference() = 0;
};