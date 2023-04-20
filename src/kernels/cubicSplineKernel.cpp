#include "cubicSplineKernel.hpp"

float CubicSplineKernel::kernel(glm::vec2 diff) const {
    float q = glm::length(diff) / kernelRadius;
    if (q <= 0.5f)
        return  normalizationFactor * (6 * (q*q*q - q*q) + 1);
    if (q <= 1.f) {
        float tmp = 1 - q;
        return normalizationFactor * (2 * tmp*tmp*tmp);
    }
    return 0.f;
}

glm::vec2 CubicSplineKernel::gradientKernel(glm::vec2 diff) const {
    float q = glm::length(diff) / kernelRadius;
    if (q <= 0.5f)
        return ((6 * normalizationFactor) / kernelRadius) * (3*q*q - 2*q) * glm::normalize(diff);
    if (q <= 1.f) {
        float tmp = 1 - q;
        return -1 * ((6 * normalizationFactor) / kernelRadius) * tmp*tmp * glm::normalize(diff);
    }
    return glm::vec2(0.f);
}