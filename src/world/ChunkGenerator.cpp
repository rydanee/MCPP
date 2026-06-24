//
// Created by rydanee on 6/24/26.
//

#include "ChunkGenerator.h"

float PseudoPerlin2D(float x, float z)
{
    float value = sin(x) * cos(z) + sin(x * 0.5f) * cos(z * 2.1f);
    return (value + 2.0f) / 4.0f;
}

ChunkGenerator::ChunkGenerator(int seed) {
    this->seed = seed;
}

#include <future>
#include <vector>

void ChunkGenerator::GenerateChunks() {
    int offset = chunkRadius / 2;
    int totalChunks = chunkRadius * chunkRadius;
    chunks.resize(totalChunks);

    std::vector<ChunkGenerator::ChunkDataDynamic> chunkDataList(totalChunks);

    std::vector<std::future<void>> genFutures;
    genFutures.reserve(totalChunks);

    for (int x = 0; x < chunkRadius; x++) {
        for (int z = 0; z < chunkRadius; z++) {
            int idx = x * chunkRadius + z;
            genFutures.push_back(std::async(std::launch::async, [this, &chunkDataList, x, z, idx, offset]() {
                chunkDataList[idx] = GenerateChunkData(x - offset, z - offset);
            }));
        }
    }

    for (auto& f : genFutures) {
        f.get();
    }

    std::vector<std::future<std::unique_ptr<Chunk>>> meshFutures;
    meshFutures.reserve(totalChunks);

    for (int x = 0; x < chunkRadius; x++) {
        for (int z = 0; z < chunkRadius; z++) {
            int idx = x * chunkRadius + z;
            glm::vec3 worldPos((x - offset) * chunkSize, 0.0f, (z - offset) * chunkSize);

            meshFutures.push_back(std::async(std::launch::async, [this, &chunkDataList, worldPos, idx]() {
                auto chunk = std::make_unique<Chunk>(worldPos, chunkSize, chunkHeight, nullptr);

                chunk->GenerateMesh(chunkDataList, idx, chunkRadius);

                return chunk;
            }));
        }
    }

    for (int i = 0; i < totalChunks; i++) {
        chunks[i] = meshFutures[i].get();
        if (chunks[i]) {
            chunks[i]->SetBlocks(std::move(chunkDataList[i]));
        }
    }

    for (auto& chunk : chunks) {
        if (chunk) {
            chunk->UploadToGPU();
        }
    }
}

void ChunkGenerator::RenderChunks() {
    for (auto& chunk : chunks) {
        if (chunk) {
            chunk->Render();
        }
    }
}

ChunkGenerator::ChunkDataDynamic ChunkGenerator::GenerateChunkData(int cx, int cy) {
    int totalBlocks = chunkSize * chunkHeight * chunkSize;
    auto blockData = std::make_unique<BlockData::BlockType[]>(totalBlocks);

    auto setBlock = [&](int x, int y, int z, BlockData::BlockType type) {
        int idx = x + chunkSize * (y + chunkHeight * z);
        blockData[idx] = type;
    };

    int wx = cx * chunkSize;
    int wz = cy * chunkSize;

    for (int x = 0; x < chunkSize; x++) {
        for (int z = 0; z < chunkSize; z++) {
            int gx = wx + x;
            int gz = wz + z;
            int terrainHeight = CalculateHeight(gx, gz);

            for (int y = 0; y < chunkHeight; y++) {
                if (y > terrainHeight) {
                    setBlock(x, y, z, BlockData::BlockType::AIR);
                } else if (y == terrainHeight) {
                    setBlock(x, y, z, BlockData::BlockType::GRASS);
                } else if (terrainHeight - y > 5) {
                    setBlock(x, y, z, BlockData::BlockType::DIRT);
                } else {
                    setBlock(x, y, z, BlockData::BlockType::STONE);
                }
            }
        }
    }
    return blockData;
}

int ChunkGenerator::CalculateHeight(int gx, int gz) {
    float sex = (gx + seed) * noisescale;
    float sez = (gz + seed) * noisescale;

    float baseNoise = PseudoPerlin2D(sex, sez);
    float detailNoise = PseudoPerlin2D(sex * 4.0f, sez * 4.0f);

    if (detailNoise == 0.0f) detailNoise = 0.001f;
    float combinedNoise = (baseNoise / detailNoise) / 1.2f;

    int minHeight = 5;
    int maxHeight = 60;

    int calculated = (int)(minHeight + (combinedNoise * (maxHeight - minHeight)));
    return glm::clamp(calculated, 0, chunkHeight - 1);
}
