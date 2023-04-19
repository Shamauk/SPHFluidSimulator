#pragma once

#include "simulator.hpp"
#include "../kernels/solenthalerKernel.hpp"

class MullerSimulator : public Simulator {
public:
    MullerSimulator(float viewWidth, float viewHeight, float particleRadius) : Simulator("Muller Simulator", 1) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->kernel = new SolenthalerKernel(2 * particleRadius);
        this->boundary = new PositionalBoundary();
        this->boundary->resetBoundary(viewWidth, viewHeight);
        this->discretization = new BruteDiscretization(viewWidth, viewHeight, kernel->getKernelRange());
    }

    void update(ConstVectorWrapper<Particle>) override;

    void resetBoundary() override {
        boundary->resetBoundary(this->viewWidth, this->viewHeight);
    }

    float getKernelRange() override {
        return kernel->getKernelRange();
    }

    std::vector<Parameter> getParameters() override {
        return std::vector<Parameter>{
            Parameter{"Time step", &this->DT, 0.0000001f, 0.0001f},
            Parameter{"Kernel Range", kernel->getKernelParameterReference(), 0.01f, 40.f},
            Parameter{"Gravity", &this->ACCELERATION_DUE_TO_GRAVITY.y, -25.0f, 25.0f},
            Parameter{"Rest Density", &this->REST_DENSITY, 0.f, 1000.f},
            Parameter{"Pressure Stiffness", &this->PRESSURE_STIFFNESS, 1.f, 10000.f},
            Parameter{"Mass", &this->MASS, 0.f, 100.f},
            Parameter{"Viscosity Coefficient", &this->VISCOSITY_FACTOR, 1.f, 3000.f},
        };
    }
private:
    // PARAMS
    float viewWidth;
    float viewHeight;
    glm::vec2 ACCELERATION_DUE_TO_GRAVITY = glm::vec2(0.0f, -9.81f);          
    float REST_DENSITY = 300.f;          
    float PRESSURE_STIFFNESS = 4000.f;          
    float MASS = 2.5f;		           
    float VISCOSITY_FACTOR = 200.f;	           
    float DT = 0.0007f;	      

    // Objects
    SolenthalerKernel *kernel;

    // METHODS
    void Integrate(ConstVectorWrapper<Particle>);
    void ComputeDensityPressure(ConstVectorWrapper<Particle>);
    void ComputeForces(ConstVectorWrapper<Particle>);
};