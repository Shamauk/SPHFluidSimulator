#pragma once

#include <vector>

#include "discretization.hpp"

class GridDiscretization : public Discretization {
public:
    GridDiscretization(float viewWidth, float viewHeight, float kernelRange)
        : Discretization(viewWidth, viewHeight, kernelRange, "Grid") {
            indexMultiplier = 1.f / kernelRange;

            numRows = (int) (viewWidth * indexMultiplier);
            numCols = (int) (viewHeight * indexMultiplier);

            grid = new std::vector<Particle *> *[numRows];

            for (int i = 0; i < numRows; i++) {
                grid[i] = new std::vector<Particle *> [numCols];
            }
        }
    
    void getNeighbors(ConstVectorWrapper<Particle>) override;
    void clearDatastructure();
    void loadDatastructure(ConstVectorWrapper<Particle>);
    void assignNeighbors(ConstVectorWrapper<Particle>);


    size_t getVectorSizeInBytes(const std::vector<Particle *>& v) {
        return sizeof(Particle *) * v.capacity();
    }

    size_t getMemoryUse() override {
        size_t totalSize = sizeof(grid);
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols; j++) {
                totalSize += getVectorSizeInBytes(grid[i][j]);
                totalSize += sizeof(grid[i][j]);
            }
            totalSize += sizeof(grid[i]);
        }
        return totalSize;
    }

private:
    float indexMultiplier;
    int numRows;
    int numCols;

    std::vector<Particle *> **grid;
};