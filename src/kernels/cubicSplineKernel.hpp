#pragma once

#include "kernel.hpp"

class CubicSplineKernel : public Kernel {
public:
    CubicSplineKernel(float kernelRadius) {
        this->kernelRadius = kernelRadius;
        this->normalizationFactor = 40.f / (7 * M_PI * kernelRadius*kernelRadius);
    }

    float getKernelRange() override {
        return kernelRadius;
    }

    float *getKernelParameterReference() override {
        return &kernelRadius;
    }

    float kernel(glm::vec2) const;
    glm::vec2 gradientKernel(glm::vec2) const;
private:
    float kernelRadius;
    float normalizationFactor;
};