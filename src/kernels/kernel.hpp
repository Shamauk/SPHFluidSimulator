#pragma once

class Kernel {
public:
    virtual float getKernelRange() = 0;
    virtual float *getKernelParameterReference() = 0;
};