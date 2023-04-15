#ifndef KERNEL_H
#define KERNEL_H

#include "glm/glm.hpp"

class Kernel {
public: 
    Kernel(double h);
    double getKernelValue(glm::dvec2& r);
    glm::dvec2 getKernelGradient(glm::dvec2& r);
    double poly6(glm::dvec2& r);
    double spiky(glm::dvec2& r);
    double viscosity(glm::dvec2& r);
    double laplacianViscosity(glm::dvec2& r);
private:
    double h;
    double h5;
    double h8;
    double sigma;
};

#endif
