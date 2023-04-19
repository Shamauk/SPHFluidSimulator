#pragma once

#include <string>

#include "../datastructures/particle.hpp"
#include "../utils/constVector.hpp"

// Boundaries
#include "../boundaries/positionalBoundary.hpp"

// Discretizations
#include "../discretizations/bruteDiscretization.hpp"
#include "../discretizations/gridDiscretization.hpp"
#include "../discretizations/compactHashingDiscretization.hpp"

struct Parameter {
    std::string name;
    float* value;
    float min;
    float max;
};

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
    Boundary *boundary;
    Discretization *discretization;
};