#pragma once

#include "simulator.hpp"

class MullerSimulator : public Simulator {
public:
    MullerSimulator(float viewWidth, float viewHeight, float particleRadius, Boundary *boundary) : Simulator("Muller", 1) {
        this->viewWidth = viewWidth;
        this->viewHeight = viewHeight;
        this->kernel = new SolenthalerKernel(2 * particleRadius);
        this->boundary = boundary;
        this->discretization = new BruteDiscretization(viewWidth, viewHeight, kernel->getKernelRange());
        this->DT = 0.0007f;
    }

    void update(ConstVectorWrapper<Particle>) override;

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
    float viewWidth;
    float viewHeight;

    // PARAMS
    glm::vec2 ACCELERATION_DUE_TO_GRAVITY = glm::vec2(0.0f, -9.81f);          
    float REST_DENSITY = 300.f;          
    float PRESSURE_STIFFNESS = 4000.f;          
    float MASS = 2.5f;		           
    float VISCOSITY_FACTOR = 200.f;	           

    // Objects
    SolenthalerKernel *kernel;

    // METHODS
    void Integrate(ConstVectorWrapper<Particle>);
    void ComputeDensityPressure(ConstVectorWrapper<Particle>);
    void ComputeForces(ConstVectorWrapper<Particle>);
};