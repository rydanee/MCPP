//
// Created by rydanee on 6/24/26.
//

#include "ChunkGenerator.h"

#include <iostream>

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

    chunks.clear();
    chunks.resize(totalChunks);

    std::vector<ChunkGenerator::ChunkDataDynamic> chunkDataList(totalChunks);
    for (int x = 0; x < chunkRadius; x++) {
        for (int z = 0; z < chunkRadius; z++) {
            int idx = x * chunkRadius + z;
            chunkDataList[idx] = GenerateChunkData(x - offset, z - offset);
        }
    }

    for (int x = 0; x < chunkRadius; x++) {
        for (int z = 0; z < chunkRadius; z++) {
            int idx = x * chunkRadius + z;
            glm::vec3 worldPos((x - offset) * chunkSize, 0.0f, (z - offset) * chunkSize);

            auto chunk = std::make_unique<Chunk>(worldPos, chunkSize, chunkHeight, nullptr);

            chunk->GenerateMesh(chunkDataList, idx, chunkRadius);

            chunks[idx] = std::move(chunk);
        }
    }

    for (int x = 0; x < chunkRadius; x++) {
        for (int z = 0; z < chunkRadius; z++) {
            int idx = x * chunkRadius + z;

            if (chunks[idx]) {
                chunks[idx]->SetBlocks(std::move(chunkDataList[idx]));

                chunks[idx]->UploadToGPU();
            }
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

BlockData::BlockType ChunkGenerator::GetBlockAtGlobalPos(const glm::ivec3& globalPos) {
    if (globalPos.y < 0 || globalPos.y >= chunkHeight) {
        return BlockData::BlockType::AIR;
    }

    auto floorDiv = [](int a, int b) -> int {
        int q = a / b;
        int r = a % b;
        if (r != 0 && ((a < 0) != (b < 0))) {
            q--;
        }
        return q;
    };

    int cx = floorDiv(globalPos.x, chunkSize);
    int cz = floorDiv(globalPos.z, chunkSize);

    int offset = chunkRadius / 2;
    int arrayX = cx + offset;
    int arrayZ = cz + offset;

    if (arrayX < 0 || arrayX >= chunkRadius || arrayZ < 0 || arrayZ >= chunkRadius) {
        return BlockData::BlockType::AIR;
    }

    int chunkIdx = arrayX * chunkRadius + arrayZ;

    if (chunkIdx < 0 || chunkIdx >= static_cast<int>(chunks.size()) || !chunks[chunkIdx]) {
        return BlockData::BlockType::AIR;
    }

    auto floorMod = [](int a, int b) -> int {
        int r = a % b;
        if (r < 0) r += b;
        return r;
    };

    int bx = floorMod(globalPos.x, chunkSize);
    int bz = floorMod(globalPos.z, chunkSize);
    int by = globalPos.y;

    int flatBlockIdx = bx + chunkSize * (by + chunkHeight * bz);

    return chunks[chunkIdx]->GetBlockAtLocalPos(flatBlockIdx);
}

ChunkGenerator::RaycastResult ChunkGenerator::Raycast(const glm::vec3& start, const glm::vec3& direction, float maxDistance) {
    RaycastResult result;

    glm::vec3 dir = glm::normalize(direction);
    if (glm::length(dir) < 0.1f) return result;

    glm::ivec3 blockPos(
        static_cast<int>(std::floor(start.x)),
        static_cast<int>(std::floor(start.y)),
        static_cast<int>(std::floor(start.z))
    );

    int stepX = (dir.x > 0.0001f) ? 1 : ((dir.x < -0.0001f) ? -1 : 0);
    int stepY = (dir.y > 0.0001f) ? 1 : ((dir.y < -0.0001f) ? -1 : 0);
    int stepZ = (dir.z > 0.0001f) ? 1 : ((dir.z < -0.0001f) ? -1 : 0);

    float tDeltaX = (stepX != 0) ? std::abs(1.0f / dir.x) : std::numeric_limits<float>::max();
    float tDeltaY = (stepY != 0) ? std::abs(1.0f / dir.y) : std::numeric_limits<float>::max();
    float tDeltaZ = (stepZ != 0) ? std::abs(1.0f / dir.z) : std::numeric_limits<float>::max();

    float tMaxX, tMaxY, tMaxZ;

    if (stepX > 0) {
        tMaxX = (float(blockPos.x + 1) - start.x) * tDeltaX;
    } else if (stepX < 0) {
        tMaxX = (start.x - float(blockPos.x)) * tDeltaX;
    } else {
        tMaxX = std::numeric_limits<float>::max();
    }

    if (stepY > 0) {
        tMaxY = (float(blockPos.y + 1) - start.y) * tDeltaY;
    } else if (stepY < 0) {
        tMaxY = (start.y - float(blockPos.y)) * tDeltaY;
    } else {
        tMaxY = std::numeric_limits<float>::max();
    }

    if (stepZ > 0) {
        tMaxZ = (float(blockPos.z + 1) - start.z) * tDeltaZ;
    } else if (stepZ < 0) {
        tMaxZ = (start.z - float(blockPos.z)) * tDeltaZ;
    } else {
        tMaxZ = std::numeric_limits<float>::max();
    }

    glm::ivec3 lastNormal(0);
    float distance = 0.0f;

    for (int i = 0; i < 256; i++) {  // Safety limit
        if (distance > maxDistance) break;

        BlockData::BlockType type = GetBlockAtGlobalPos(blockPos);

        if (type != BlockData::BlockType::AIR) {
            result.hit = true;
            result.blockPos = blockPos;
            result.normal = lastNormal;
            return result;
        }

        if (tMaxX < tMaxY && tMaxX < tMaxZ) {
            blockPos.x += stepX;
            distance = tMaxX;
            tMaxX += tDeltaX;
            lastNormal = glm::ivec3(-stepX, 0, 0);
        } else if (tMaxY < tMaxX && tMaxY < tMaxZ) {
            blockPos.y += stepY;
            distance = tMaxY;
            tMaxY += tDeltaY;
            lastNormal = glm::ivec3(0, -stepY, 0);
        } else {
            blockPos.z += stepZ;
            distance = tMaxZ;
            tMaxZ += tDeltaZ;
            lastNormal = glm::ivec3(0, 0, -stepZ);
        }
    }

    return result;
}