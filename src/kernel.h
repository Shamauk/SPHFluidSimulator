#ifndef KERNEL_H
#define KERNEL_H

#include "glm/glm.hpp"

namespace Kernel {
    double cubicSpline(double q);
    double gradCubicSpline(double q);
    double getKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h);
    glm::vec<3,double> getGradientKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h);
}

#endif