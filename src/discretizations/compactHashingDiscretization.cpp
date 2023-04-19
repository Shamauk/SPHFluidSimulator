#include "compactHashingDiscretization.hpp"

void CompactHashingDiscretization::getNeighbors(ConstVectorWrapper<Particle> particles) {
    clearDatastructure();
    loadDatastructure(particles);
    assignNeighbors(particles);
}

void CompactHashingDiscretization::clearDatastructure() {
    map.clear();
}

void CompactHashingDiscretization::loadDatastructure(ConstVectorWrapper<Particle> particles) {
    for (auto &p: particles) {
        int xIndex = p.getPosition().x * indexMultiplier;
        int yIndex = p.getPosition().y * indexMultiplier;
        if (xIndex < 0 || xIndex > numRows - 1 || yIndex < 0 || yIndex > numCols - 1)
            continue;
            
        map[calculateIndex(xIndex, yIndex)].push_back(&p);
    }
}

void CompactHashingDiscretization::assignNeighbors(ConstVectorWrapper<Particle> particles) {
    for (auto &pi : particles) {
        pi.getNeigbors().clear();

        int x = (int) (pi.getPosition().x * indexMultiplier);
        int y = (int) (pi.getPosition().y * indexMultiplier);

        if (x < 0 || x > numRows - 1 || y < 0 || y > numCols - 1)
            continue;

        int xLeftIndex = x - 1 < 0 ? x : x-1;
        int xRightIndex = x + 1 > numRows - 1 ? x : x+1;
        int yBottomIndex = y - 1 < 0 ? y : y-1;
        int yUpIndex = y + 1 > numCols - 1 ? y : y+1;

        for (int i = xLeftIndex; i <= xRightIndex; i++) {
            for (int j = yBottomIndex; j <= yUpIndex; j++) {
                int index = calculateIndex(i, j);
                for (auto *pj : map[index]) {
                    if (&pi == pj)
                        continue;
                    if (glm::length(pi.getPosition() - pj->getPosition()) <= kernelRange)
                        pi.getNeigbors().push_back(pj);
                }
            }
        }
    }
}

int CompactHashingDiscretization::calculateIndex(int i, int j) {
    return (P1 * i + P2 * j) % HASH_TABLE_SIZE;
}