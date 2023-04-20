#pragma once

#include <string>

#include "../datastructures/particle.hpp"
#include "../datastructures/parameter.hpp"
#include "../utils/constVector.hpp"

// Boundaries
#include "../boundaries/positionalBoundary.hpp"

// Discretizations
#include "../discretizations/bruteDiscretization.hpp"
#include "../discretizations/gridDiscretization.hpp"
#include "../discretizations/compactHashingDiscretization.hpp"

// Kernels
#include "../kernels/solenthalerKernel.hpp"
#include "../kernels/cubicSplineKernel.hpp"
#include "../kernels/nonNormalizedCubicSplineKernel.hpp"


class Simulator {
public:
    Simulator(std::string name, short ID) {
        this->name = name;
        this->ID = ID;
    }

    virtual void update(ConstVectorWrapper<Particle>) = 0;
    virtual void resetBoundary() = 0;

    const std::string getName() {
        return name;
    }

    virtual std::vector<Parameter> getParameters() {
        return std::vector<Parameter>();
    }

    float getTimeStep() {
        return DT;
    }

    virtual float getKernelRange() = 0;

    void setDiscretization(Discretization *discretization) {
        this->discretization = discretization;
    }

    short getID() {
        return this->ID;
    }

    std::string getDiscretizationName() {
        return discretization->getDiscretizationName();
    }

    size_t getDiscretizationMemoryUsage() {
        return discretization->getMemoryUse();
    }

    void setBoundaryX(float x) {
        boundary->setMaxX(x);
    }

    void setBoundaryY(float y) {
        boundary->setMaxY(y);
    }

private:
    std::string name = "Specimen 462";
protected:
    short ID;
    float DT = 0.0001;

    Boundary *boundary;
    Discretization *discretization;
};