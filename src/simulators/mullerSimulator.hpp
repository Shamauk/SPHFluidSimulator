#pragma once

#include "../simulator.hpp"
#include "../kernels/solenthalerKernel.hpp"

class MullerSimulator : public Simulator {
public:
    MullerSimulator(float viewWidth, float viewHeight, float particleRadius) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->kernelRadius = 2 * particleRadius;
        this->solenthalerKernel = SolenthalerKernel(this->kernelRadius);
        this->epsilon = this->kernelRadius;
    }

    void update(ConstVectorWrapper<Particle>) override;
private:
    // PARAMS
    float viewWidth;
    float viewHeight;
    float kernelRadius;
    const glm::vec2 ACCELERATION_DUE_TO_GRAVITY = glm::vec2(0.0f, -9.81f);          
    const float REST_DENSITY = 300.f;          
    const float PRESSURE_STIFFNESS = 2000.f;          
    const float MASS = 2.5f;		           
    const float VISCOSITY_FACTOR = 200.f;	           
    const float DT = 0.0007f;	      

    // BOUNDARIES
    float epsilon;
    const float BOUND_DAMPING = -0.5f;

    // OBJECTS
    SolenthalerKernel solenthalerKernel;

    // METHODS
    void Integrate(ConstVectorWrapper<Particle>);
    void ComputeDensityPressure(ConstVectorWrapper<Particle>);
    void ComputeForces(ConstVectorWrapper<Particle>);
};