#include "kernel.h"
#include <cmath>

namespace Kernel {
    double getKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
        double dist = glm::distance(xi, xj);
        return (cubicSpline(dist/h)) / (h*h*h); 
    }

    glm::vec<3,double> getGradientKernelValue(const glm::vec<3, double>& xi, const glm::vec<3, double>& xj, double h) {
        double dist = glm::distance(xi, xj);
        return (gradCubicSpline(dist/h)*((xi-xj)/dist)) / (h*h*h);
    }

    double gradCubicSpline(double q) {
        double result = 3 / (2 * M_PI);
        if (q < 1) {
            result *= (-2 * q  +  1.5 * q * q);
        } else if (q < 2) {
            result *= (-0.5 * (2-q) * (2-q));
        } else {
            result = 0;
        }
        return result;
    }

    double cubicSpline(double q) {
        double result = 3 / (2 * M_PI);
        if (q < 1) {
            result *= (2/3 - q*q + 0.5*q*q*q);
        } else if (q < 2) {
            result *= (1/6 * (2-q) * (2-q) * (2-q));
        } else {
            result *= 0;
        }
        return result;
    }
}