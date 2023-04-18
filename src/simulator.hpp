#pragma once

#include <string>

#include "particle.hpp"
#include "utils/constVector.hpp"

struct Parameter {
    std::string name;
    float* value;
    float min;
    float max;
};

class Simulator {
public:
    Simulator(std::string name) {
        this->name = name;
    }

    virtual void update(ConstVectorWrapper<Particle>) = 0;

    const std::string getName() {
        return name;
    }

    virtual std::vector<Parameter> getParameters() {
        return std::vector<Parameter>();
    }
private:
    std::string name = "Specimen 462";
};