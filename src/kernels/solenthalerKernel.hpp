#pragma once

#include <glm/glm.hpp>
#include <cmath>

class SolenthalerKernel {
public:
    SolenthalerKernel() {
        SolenthalerKernel(16.f);
    }
    
    SolenthalerKernel(float kernelRadius) {
        this->kernelRadius = kernelRadius;
        this->kernelRadiusSecondPower = this->kernelRadius * this->kernelRadius;
        this->kernelRadiusFifthPower = this->kernelRadiusSecondPower * 
            this->kernelRadiusSecondPower * this->kernelRadius;
        this->kernelRadiusEightPower = this->kernelRadiusFifthPower * 
            this->kernelRadiusSecondPower * this->kernelRadius;
    }

    float laplacianViscosityKernel(glm::vec2) const;
    glm::vec2 gradientSpikyKernel(glm::vec2) const;
    float poly6Kernel(glm::vec2) const;

private:
    float kernelRadius;
    float kernelRadiusSecondPower;
    float kernelRadiusFifthPower;
    float kernelRadiusEightPower;
};