#include <iostream>
#include <cmath>
#include "kernel.h"

Kernel kernelOne = Kernel(1.0);
Kernel kernelPointOne = Kernel(0.1);

bool testGetKernelValueItself() {
    glm::dvec3 r = glm::dvec3(0.0);
   
    double result = kernelOne.getKernelValue(r);
    double expected = 8 / M_PI;

    return abs(result - expected) < DBL_EPSILON;
}

bool testGetKernelValueHalfSmoothingLength() {
    glm::dvec3 pos1 = glm::dvec3(0.3,0.0,0.3);
    glm::dvec3 pos2 = glm::dvec3(0.0,-0.3,0.04);
    glm::dvec3 r = pos1 - pos2;

    double result = kernelOne.getKernelValue(r);

    double q = sqrt(0.2476);
    double expected = (8/M_PI) * (6 * (q*q*q - q*q) + 1);

    return abs(result - expected) < DBL_EPSILON;
}

bool testGetKernelValueAtSmoothingLength() {
    glm::dvec3 r = glm::dvec3(0.577, 0.577, 0.577);

    double result = kernelOne.getKernelValue(r);

    double q = sqrt(3*r.x*r.x);
    double expected = (8/M_PI) * (2 * (1-q)*(1-q)*(1-q));

    return abs(result - expected) < DBL_EPSILON;
}

bool testGetKernelValueAtMixedPoint() {
    glm::dvec3 r = glm::dvec3(0.2, 0.35, 0.45);

    double result = kernelOne.getKernelValue(r);

    double q = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
    double expected = (8/M_PI) * (2 * (1-q)*(1-q)*(1-q));

    return abs(result - expected) < DBL_EPSILON;
}

bool testGetKernelValueOutsideSmoothing() {
    glm::dvec3 r = glm::dvec3(0.6, 0.6, 0.6);

    double result = kernelOne.getKernelValue(r);

    double q = 0.0;

    return result == q;
}

bool testGetKernelValueAtMixedPointNonUnitKernel() {
    glm::dvec3 r = glm::dvec3(0.02, 0.035, 0.045);

    double result = kernelPointOne.getKernelValue(r);

    double q = sqrt(r.x*r.x + r.y*r.y + r.z*r.z) / 0.1;
    double expected = (8/(M_PI*0.1*0.1*0.1)) * (2 * (1-q)*(1-q)*(1-q));

    return abs(result - expected) < DBL_EPSILON;
}


bool testGetKernelGradientItself() {
    glm::dvec3 r = glm::dvec3(0.0);

    glm::dvec3 result = kernelOne.getKernelGradient(r);
    
    glm::dvec3 expected = glm::dvec3(0.0);

    return abs(result.x - expected.x) < DBL_EPSILON && 
            abs(result.y - expected.y) < DBL_EPSILON &&
            abs(result.z - expected.z) < DBL_EPSILON;
}

bool testGetKernelGradientHalfSmoothingLength() {
    glm::dvec3 r = glm::dvec3(0.3,-0.3,0.26);

    glm::dvec3 result = kernelOne.getKernelGradient(r);

    double q = sqrt(0.2476);
    glm::dvec3 expected = (48/M_PI) * (3*q*q - 2*q) * (r/q);

    return abs(result.x - expected.x) < FLT_EPSILON && 
            abs(result.y - expected.y) < FLT_EPSILON &&
            abs(result.z - expected.z) < FLT_EPSILON;
}

bool testGetKernelGradientAtMixedPoint() {
    glm::dvec3 r = glm::dvec3(0.2, 0.35, 0.45);

    glm::dvec3 result = kernelOne.getKernelGradient(r);

    double q = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
    glm::dvec3 expected = -1.0 * (48/M_PI) * (1-q)*(1-q) * (r/q);

    return abs(result.x - expected.x) < FLT_EPSILON && 
            abs(result.y - expected.y) < FLT_EPSILON &&
            abs(result.z - expected.z) < FLT_EPSILON;
}

bool testGetKernelGradientOutsideSmoothing() {
    glm::dvec3 r = glm::dvec3(0.6, 0.6, 0.6);

    glm::dvec3 result = kernelOne.getKernelGradient(r);

    glm::dvec3 expected = glm::dvec3(0.0,0.0,0.0);

    return abs(result.x - expected.x) < FLT_EPSILON && 
            abs(result.y - expected.y) < FLT_EPSILON &&
            abs(result.z - expected.z) < FLT_EPSILON;
}

bool testGetKernelGradientAtMixedPointNonUnitKernel() {
    glm::dvec3 r = glm::dvec3(0.02, 0.005, 0.045);

    glm::dvec3 result = kernelPointOne.getKernelGradient(r);

    double rLength = sqrt(r.x*r.x + r.y*r.y + r.z*r.z);
    double q = rLength / 0.1;
    glm::dvec3 expected = ((48)/(M_PI *0.1*0.1*0.1*0.1)) * (3*q*q - 2*q) * (r / rLength);

    return abs(result.x - expected.x) < FLT_EPSILON && 
            abs(result.y - expected.y) < FLT_EPSILON &&
            abs(result.z - expected.z) < FLT_EPSILON;
}


int main(int argc, char **argv) {
    // Run the tests and check the results
    int failure = 0;
    if (testGetKernelValueItself()) {
        std::cout << "testGetKernelValueItself PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelValueItself FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelValueHalfSmoothingLength()) {
        std::cout << "testGetKernelValueHalfSmoothingLength PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelValueHalfSmoothingLength FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelValueAtSmoothingLength()) {
        std::cout << "testGetKernelValueAtSmoothingLength PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelValueAtSmoothingLength FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelValueAtMixedPoint()) {
        std::cout << "testGetKernelValueAtMixedPoint PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelValueAtMixedPoint FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelValueOutsideSmoothing()) {
        std::cout << "testGetKernalValueOutsideSmoothing PASSED" << std::endl;
    } else {
        std::cout << "testGetKernalValueOutsideSmoothing FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelValueAtMixedPointNonUnitKernel()) {
        std::cout << "testGetKernelValueAtMixedPointNonUnitKernel PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelValueAtMixedPointNonUnitKernel FAILED" << std::endl;
        failure = 1;
    }

    if (testGetKernelGradientItself()) {
        std::cout << "testGetKernelGradientItself PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelGradientItself FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelGradientHalfSmoothingLength()) {
        std::cout << "testGetKernelGradientHalfSmoothingLength PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelGradientHalfSmoothingLength FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelGradientAtMixedPoint()) {
        std::cout << "testGetKernelGradientAtMixedPoint PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelGradientAtMixedPoint FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelGradientOutsideSmoothing()) {
        std::cout << "testGetKernelGradientOutsideSmoothing PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelGradientOutsideSmoothing FAILED" << std::endl;
        failure = 1;
    }
    if (testGetKernelGradientAtMixedPointNonUnitKernel()) {
        std::cout << "testGetKernelGradientAtMixedPointNonUnitKernel PASSED" << std::endl;
    } else {
        std::cout << "testGetKernelGradientAtMixedPointNonUnitKernel FAILED" << std::endl;
        failure = 1;
    }

    return failure;
}
