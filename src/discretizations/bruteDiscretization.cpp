#include "bruteDiscretization.hpp"

void BruteDiscretization::getNeighbors(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
        pi.getNeigbors().clear();
        for (auto &pj : particles) {
            if (&pi == &pj) continue;
            if (glm::length(pi.getPosition() - pj.getPosition()) <= kernelRange) {
                pi.getNeigbors().push_back(&pj);
            }
        }
    }
} 