#include "kernel.h"
#include <cmath>
#include <iostream>

double Kernel::getKernelValue(glm::dvec3& r) {
    double q = glm::length(r) / h;
    double multiplier = 0;
    if (q <= 0.5) {
        multiplier = 6 * (q*q*q - q*q) + 1;
    } else if (q <= 1) {
        double tmp = 1 - q;
        multiplier = 2 * tmp*tmp*tmp;
    }
    return this->sigma * multiplier;
}

glm::dvec3 Kernel::getKernelGradient(glm::dvec3& r) {
    double lengthR = glm::length(r);
    if (lengthR == 0) {
        return glm::dvec3(0.0);
    }
    double q = lengthR / h;
    glm::dvec3 multiplier = glm::dvec3(0.0);
    if (q <= 0.5) {
        multiplier = (3*q*q - 2*q) * (r / lengthR);
    } else if (q <= 1) {
        double tmp = 1 - q;
        multiplier = -1 * tmp*tmp * (r / lengthR);
    }
    return ((6 * this->sigma) / h) * multiplier;
}

Kernel::Kernel(double h) {
    this->h = h;
    this->sigma = 8.0 / (M_PI * h*h*h);
}

