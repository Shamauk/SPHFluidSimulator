#include "solenthalerKernel.hpp"

float SolenthalerKernel::laplacianViscosityKernel(glm::vec2 difference) const {
    return (40.f / (M_PI * kernelRadiusFifthPower)) * (kernelRadius - glm::length(difference));
}

glm::vec2 SolenthalerKernel::gradientSpikyKernel(glm::vec2 difference) const {
    float length = glm::length(difference);
    float tmp = kernelRadius - length;
    return (float)((-30.f / (M_PI * kernelRadiusFifthPower)) * tmp*tmp) * (difference / length);
}

float SolenthalerKernel::poly6Kernel(glm::vec2 difference) const {
    float length = glm::length(difference);
    float tmp = kernelRadiusSecondPower - length*length;
    return (4.f / (M_PI * kernelRadiusEightPower)) * tmp*tmp*tmp;
}