#include "kernel.h"
#include <cmath>
#include <iostream>

namespace Kernel {
    double getKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
        double dist = glm::distance(xi, xj);
        return (cubicSpline(dist/h)) / (h*h*h); 
    }

    double cubicSpline(double q) {
        double result = 3 / (2 * M_PI);
        if (q < 1) {
            result *= ((2/3) - q*q + 0.5*q*q*q);
        } else if (q < 2) {
            result *= (((2-q)*(2-q)*(2-q)) / 6);
        } else {
            result *= 0;
        }
        return result;
    }


    glm::vec<3,double> getGradientKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
        double dist = glm::distance(xi, xj);
        return (gradCubicSpline(dist/h)*((xi-xj)/dist)) / (h*h*h);
    }

    double gradCubicSpline(double q) {
        double result = 3 / (2 * M_PI);
        if (q < 1) {
            result *= (-2*q + 1.5*q*q);
        } else if (q < 2) {
            result *= (-0.5*(2-q)*(2-q));
        } else {
            result = 0;
        }
        return result;
    }

    double poly6Kernel(glm::dvec3& r, double h) {
        double rNorm = glm::length(r);
        if (rNorm > h) {
            return 0.0;
        }
        double term = h*h - rNorm*rNorm;
        return (315/(64 * M_PI * h*h*h*h*h*h*h*h*h)) * term*term*term;
    }

    glm::dvec3 spikyKernelGradient(glm::dvec3& r, double h) {
        double rNorm = glm::length(r);
        if (rNorm > h || rNorm < 1e-10) {
            return glm::dvec3(0.0);
        }
        double term = h - rNorm;
        return -(45/(M_PI * h*h*h*h*h*h))* term*term * r/rNorm;
    }

    double viscosityKernelLaplacian(glm::dvec3& r, double h) {
        double rNorm = glm::length(r);
        if (rNorm > h) {
            return 0.0;
        }

        return (45/(M_PI * h*h*h*h*h*h)) * (h - rNorm);
    }
}