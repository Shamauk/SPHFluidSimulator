#ifndef KERNEL_H
#define KERNEL_H

#include "glm/glm.hpp"

// namespace Kernel {
//     double cubicSpline(double q);
//     double gradCubicSpline(double q);
//     double getKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h);
//     glm::vec<3,double> getGradientKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h);
//     double poly6Kernel(glm::dvec3& r, double h);
//     glm::dvec3 spikyKernelGradient(glm::dvec3& r, double h);
//     double viscosityKernelLaplacian(glm::dvec3& r, double h);
// }

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