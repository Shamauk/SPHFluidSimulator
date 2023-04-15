#ifndef KERNEL_H
#define KERNEL_H

#include "glm/glm.hpp"

class Kernel {
public: 
    Kernel(double h);
    double getKernelValue(glm::dvec2& r);
    glm::dvec2 getKernelGradient(glm::dvec2& r);
private:
    double h;
    double sigma;
};

#endif
