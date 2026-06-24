//
// Created by rydanee on 6/24/26.
//

#ifndef MCPP_CHUNKGENERATOR_H
#define MCPP_CHUNKGENERATOR_H
#include <memory>
#include <vector>

#include "BlockData.hpp"
#include "Chunk.h"

class ChunkGenerator {
public:
    ChunkGenerator(int seed);
    static inline const int chunkSize = 16;
    static inline const int chunkHeight = 140;
    void GenerateChunks();
    void RenderChunks();
    using ChunkDataDynamic = std::unique_ptr<BlockData::BlockType[]>;

    ChunkDataDynamic GenerateChunkData(int cx, int cy);

private:
    int seed;
    float noisescale = 0.0005f;
    int chunkRadius = 32;
    std::vector<std::unique_ptr<Chunk>> chunks;
    int CalculateHeight(int gx, int gy);
};


#endif //MCPP_CHUNKGENERATOR_H
