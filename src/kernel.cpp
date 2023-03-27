#include "kernel.h"
#include <cmath>
#include <iostream>


// namespace Kernel {
//     double sigma = 8.0 / ()

//     double getKernelValue(glm::dvec3& r, double h) {

//     }   

    // double getKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
    //     double dist = glm::distance(xi, xj);
    //     return (cubicSpline(dist/h)) / (h*h*h); 
    // }

    // double cubicSpline(double q) {
    //     double result = 3 / (2 * M_PI);
    //     if (q < 1) {
    //         result *= ((2/3) - q*q + 0.5*q*q*q);
    //     } else if (q < 2) {
    //         result *= (((2-q)*(2-q)*(2-q)) / 6);
    //     } else {
    //         result *= 0;
    //     }
    //     return result;
    // }


    // glm::vec<3,double> getGradientKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
    //     double dist = glm::distance(xi, xj);
    //     return (gradCubicSpline(dist/h)*((xi-xj)/dist)) / (h*h*h);
    // }

    // double gradCubicSpline(double q) {
    //     double result = 3 / (2 * M_PI);
    //     if (q < 1) {
    //         result *= (-2*q + 1.5*q*q);
    //     } else if (q < 2) {
    //         result *= (-0.5*(2-q)*(2-q));
    //     } else {
    //         result = 0;
    //     }
    //     return result;
    // }

//     double poly6Kernel(glm::dvec3& r, double h) {
//         double rNorm = glm::length(r);
//         if (rNorm > h) {
//             return 0.0;
//         }
//         double term = h*h - rNorm*rNorm;
//         return (315/(64 * M_PI * h*h*h*h*h*h*h*h*h)) * term*term*term;
//     }

//     glm::dvec3 spikyKernelGradient(glm::dvec3& r, double h) {
//         double rNorm = glm::length(r);
//         if (rNorm > h || rNorm < 1e-10) {
//             return glm::dvec3(0.0);
//         }
//         double term = h - rNorm;
//         return -(45/(M_PI * h*h*h*h*h*h))* term*term * r/rNorm;
//     }

//     double viscosityKernelLaplacian(glm::dvec3& r, double h) {
//         double rNorm = glm::length(r);
//         if (rNorm > h) {
//             return 0.0;
//         }

//         return (45/(M_PI * h*h*h*h*h*h)) * (h - rNorm);
//     }
// }

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
        multiplier = 6.0 * (3*q*q - 2*q) * (1/h) * (r / lengthR);
    } else if (q <= 1) {
        float tmp = q-1;
        multiplier = 6 * tmp*tmp * (1/h) * (r / lengthR);
    }
    return this->sigma * multiplier;
}

Kernel::Kernel(double h) {
    this->h = h;
    this->sigma = 8.0 / (M_PI * h*h*h);
}

