#include "nonNormalizedCubicSplineKernel.hpp"

float NonNormalizedCubicSplineKernel::kernel(glm::vec2 diff) const {
    float q = glm::length(diff) / kernelSmoothingLength;
    if (q <= 1)
        return  normalizationFactor * (0.67f - q*q + 0.5f*q*q*q);
    if (q <= 2) {
        float tmp = 2.f - q;
        return normalizationFactor * (0.17f * tmp*tmp*tmp);
    }
    return 0.f;
}

glm::vec2 NonNormalizedCubicSplineKernel::gradientKernel(glm::vec2 diff) const {
    float q = glm::length(diff) / kernelSmoothingLength;
    if (q <= 1)
        return normalizationFactor * (-2.f*q + 1.5f*q*q) * glm::normalize(diff);
    if (q <= 2) {
        float tmp = 2.f - q;
        return normalizationFactor * (-0.5f*tmp*tmp) * glm::normalize(diff);
    }
    return glm::vec2(0.f);
}