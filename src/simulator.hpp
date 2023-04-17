#pragma once

#include "particle.hpp"
#include "utils/constVector.hpp"

class Simulator {
public:
    virtual void update(ConstVectorWrapper<Particle>) = 0;
};