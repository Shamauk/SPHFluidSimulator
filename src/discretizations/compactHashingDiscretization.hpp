#pragma once

#include <map>

#include "discretization.hpp"

class CompactHashingDiscretization : public Discretization {
public:
    CompactHashingDiscretization(float viewWidth, float viewHeight, float kernelRange)
        : Discretization(viewWidth, viewHeight, kernelRange, "Compact Hashing") {
            indexMultiplier = 1.f / kernelRange;

            numRows = (int) (viewWidth * indexMultiplier);
            numCols = (int) (viewHeight * indexMultiplier);
        }

    void getNeighbors(ConstVectorWrapper<Particle>) override;

    void clearDatastructure();
    void loadDatastructure(ConstVectorWrapper<Particle>);
    void assignNeighbors(ConstVectorWrapper<Particle>);
    int calculateIndex(int, int);

    size_t getVectorSizeInBytes(const std::vector<Particle *>& v) {
        return sizeof(Particle *) * v.capacity();
    }

    size_t getMemoryUse() {
        size_t totalSize = sizeof(map);
        for (const auto &kv : map) {
            totalSize += sizeof(kv.first) + sizeof(kv.second) + getVectorSizeInBytes(kv.second);
        }
        return totalSize;
    }

private:
    const int P1 = 73856093;
    const int P2 = 19349663;
    const int HASH_TABLE_SIZE = 3000;
    float indexMultiplier;
    int numRows;
    int numCols;

    std::map<int, std::vector<Particle *>> map;
};