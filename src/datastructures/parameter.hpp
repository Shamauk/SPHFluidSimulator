#pragma once

#include <string>

struct Parameter {
    std::string name;
    float* value;
    float min;
    float max;
};