#pragma once

#include <string>

#include "particle.hpp"
#include "utils/constVector.hpp"
#include "boundary.hpp"

#include "boundaries/positionalBoundary.hpp"

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

    float getKernelRadius() {
        return this->kernelRadius;
    }

    short getID() {
        return this->ID;
    }
private:
    std::string name = "Specimen 462";
protected:
    short ID;
    float kernelRadius = 16.f;
    Boundary *boundary;
};