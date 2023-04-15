#include "kernel.h"
#include <cmath>
#include <tgmath.h>
#include <iostream>

double Kernel::getKernelValue(glm::dvec2& r) {
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

glm::dvec2 Kernel::getKernelGradient(glm::dvec2& r) {
    double lengthR = glm::length(r);
    if (lengthR == 0) {
        return glm::dvec3(0.0);
    }
    double q = lengthR / h;
    glm::dvec2 multiplier = glm::dvec3(0.0);
    if (q <= 0.5) {
        multiplier = (3*q*q - 2*q) * (r / lengthR);
    } else if (q <= 1) {
        double tmp = 1 - q;
        multiplier = -1 * tmp*tmp * (r / lengthR);
    }
    return ((6 * this->sigma) / h) * multiplier;
}

double Kernel::poly6(glm::dvec2& r) {
    double lengthR = glm::length(r);

    if (lengthR <= h) {
        double tmp = h*h - lengthR*lengthR;
        return (4 / (M_PI * h8)) * tmp*tmp*tmp;
    } else {
        return 0.0;
    }
}

double Kernel::spiky(glm::dvec2& r) {
    double lengthR = glm::length(r);

    if (lengthR <= h) {
        double tmp = h - lengthR;
        return (10.0 / (M_PI * h5)) * tmp*tmp*tmp;
    } else {
        return 0.0;
    }
}

double Kernel::viscosity(glm::dvec2& r) {
    double lengthR = glm::length(r);

    if (lengthR <= h) {
        return (10 / (9*M_PI*h5)) * (-4*lengthR*lengthR*lengthR + 9*h*h*lengthR - 5*h*h*h + 6*h*h*h*(log(h) - log(lengthR)));
    } else {
        return 0.0;
    }
}

double Kernel::laplacianViscosity(glm::dvec2& r) {
    double lengthR = glm::length(r);

    if (lengthR <= h) {
        return (40 / (M_PI*h5)) * (h - lengthR);
    } else {
        return 0.0;
    }
}
 
Kernel::Kernel(double h) {
    this->h = h;
    this->h5 = h*h*h*h*h;
    this->h8 = h*h*h*h*h*h*h*h;
    this->sigma = 40.0 / (7 * M_PI * h*h);
}

