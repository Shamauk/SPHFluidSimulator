#include "gridDiscretization.hpp"

void GridDiscretization::getNeighbors(ConstVectorWrapper<Particle> particles) {
    clearDatastructure();
    loadDatastructure(particles);
    assignNeighbors(particles);
}

void GridDiscretization::clearDatastructure() {
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            grid[i][j].clear();
        }
    }
}

void GridDiscretization::loadDatastructure(ConstVectorWrapper<Particle> particles) {
    for (auto &p : particles) {
        int xIndex = p.getPosition().x * indexMultiplier;
        int yIndex = p.getPosition().y * indexMultiplier;
        if (xIndex < 0 || xIndex > numRows - 1 || yIndex < 0 || yIndex > numCols - 1)
            continue;
        grid[xIndex][yIndex].push_back(&p);
    }
}

void GridDiscretization::assignNeighbors(ConstVectorWrapper<Particle> particles) {
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
                for (auto *pj : grid[i][j]) {
                    if (&pi == pj)
                        continue;
                    if (glm::length(pi.getPosition() - pj->getPosition()) <= kernelRange)
                        pi.getNeigbors().push_back(pj);
                }
            }
        }
    }
}
