#ifndef KERNEL_H
#define KERNEL_H

#include "glm/glm.hpp"

class Kernel {
public: 
    Kernel(double h);
    double getKernelValue(glm::dvec3& r);
    glm::dvec3 getKernelGradient(glm::dvec3& r);
private:
    double h;
    double sigma;
};

#endif