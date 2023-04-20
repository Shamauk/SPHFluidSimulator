#pragma once

#include "kernel.hpp"

class NonNormalizedCubicSplineKernel : public Kernel {
public:
    NonNormalizedCubicSplineKernel(float kernelRadius) {
        this->kernelSmoothingLength = kernelRadius / 2.0;
        this->normalizationFactor = 3.f / (2.f * M_PI * kernelSmoothingLength * kernelSmoothingLength);
    }

    float getKernelRange() override {
        return 2 * kernelSmoothingLength;
    }

    float *getKernelParameterReference() override {
        return &kernelSmoothingLength;
    }

    float kernel(glm::vec2) const;
    glm::vec2 gradientKernel(glm::vec2) const;
private:
    float kernelSmoothingLength;
    float normalizationFactor;
};