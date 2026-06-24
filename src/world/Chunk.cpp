//
// Created by rydanee on 6/24/26.
//

#include "Chunk.h"
#include <glad/glad.h>

Chunk::Chunk(glm::vec3 pos, int chunkSize, int chunkheight, ChunkDataDynamic data) {
    this->position = pos;
    this->chunkSize = chunkSize;
    this->chunkHeight = chunkheight;
    this->blocks = std::move(data);
    this->vao = nullptr;
}

void Chunk::GenerateMesh(const std::vector<ChunkDataDynamic>& allChunkData, int idx, int radius) {
    int totalBlocks = chunkSize * chunkHeight * chunkSize;

    vertices.clear();
    UV.clear();
    indices.clear();
    indCount = 0;

    vertices.reserve(totalBlocks * 6 * 4);
    UV.reserve(totalBlocks * 6 * 4);
    indices.reserve(totalBlocks * 6 * 6);

    if (!allChunkData[idx]) return;

    for (int x = 0; x < chunkSize; x++) {
        for (int y = 0; y < chunkHeight; y++) {
            for (int z = 0; z < chunkSize; z++) {
                int blockIdx = x + chunkSize * (y + chunkHeight * z);

                BlockData::BlockType type = allChunkData[idx][blockIdx];
                if (type == BlockData::BlockType::AIR) continue;

                glm::vec3 blockPos = position + glm::vec3(x, y, z);

                if (isNeighborEmpty(allChunkData, idx, radius, x - 1, y, z)) {
                    IntegrateFace(BlockData::Faces::LEFT, blockPos);
                    AddIndices(1);
                }
                if (isNeighborEmpty(allChunkData, idx, radius, x + 1, y, z)) {
                    IntegrateFace(BlockData::Faces::RIGHT, blockPos);
                    AddIndices(1);
                }
                if (isNeighborEmpty(allChunkData, idx, radius, x, y, z - 1)) {
                    IntegrateFace(BlockData::Faces::BACK, blockPos);
                    AddIndices(1);
                }
                if (isNeighborEmpty(allChunkData, idx, radius, x, y, z + 1)) {
                    IntegrateFace(BlockData::Faces::FRONT, blockPos);
                    AddIndices(1);
                }
                if (isNeighborEmpty(allChunkData, idx, radius, x, y - 1, z)) {
                    IntegrateFace(BlockData::Faces::BOTTOM, blockPos);
                    AddIndices(1);
                }
                if (isNeighborEmpty(allChunkData, idx, radius, x, y + 1, z)) {
                    IntegrateFace(BlockData::Faces::TOP, blockPos);
                    AddIndices(1);
                }
            }
        }
    }
}


void Chunk::IntegrateFace(BlockData::Faces face, glm::vec3 pos) {
    auto verts = BlockData::def_vertices[face];
    for (int i = 0; i < 4; i++) {
        vertices.push_back(verts[i] + pos);
    }
    UV.insert(UV.end(), BlockData::def_UV.begin(), BlockData::def_UV.end());
}

void Chunk::AddIndices(int count) {
    for (int i = 0; i < count; i++) {
        indices.push_back(0 + indCount);
        indices.push_back(1 + indCount);
        indices.push_back(2 + indCount);
        indices.push_back(2 + indCount);
        indices.push_back(3 + indCount);
        indices.push_back(0 + indCount);

        indCount += 4;
    }
}

void Chunk::UploadToGPU() {
    if (vao != nullptr) delete vao;

    vao = new VAO();
    vao->bind();

    isMeshEmpty = indices.empty();
    indexCount = static_cast<int>(indices.size());

    if (!isMeshEmpty) {
        vao->addVertexBufferObject(vertices);
        vao->addVertexBufferObject(UV);
        vao->addIndices(indices);
    }

    vertices.clear();  vertices.shrink_to_fit();
    UV.clear();        UV.shrink_to_fit();
    indices.clear();   indices.shrink_to_fit();
}

void Chunk::Render() {
    if (vao != nullptr && !isMeshEmpty) {
        vao->bind();

        vao->draw(GL_TRIANGLES);
    }
}

bool Chunk::isNeighborEmpty(const std::vector<ChunkDataDynamic>& chunkDataList,
                             int idx, int radius, int bx, int by, int bz) {
    if (by < 0 || by >= chunkHeight) return true;

    if (!chunkDataList[idx]) return true;

    auto getLocalBlock = [&](int x, int y, int z) {
        int flatIdx = x + chunkSize * (y + chunkHeight * z);
        return chunkDataList[idx][flatIdx];
    };

    if (bx >= 0 && bx < chunkSize && bz >= 0 && bz < chunkSize) {
        return getLocalBlock(bx, by, bz) == BlockData::BlockType::AIR;
    }

    int cx = idx / radius;
    int cz = idx % radius;
    int nx = cx; int nz = cz;

    if (bx < 0)               { nx = cx - 1; bx = chunkSize - 1; }
    else if (bx >= chunkSize) { nx = cx + 1; bx = 0; }
    if (bz < 0)               { nz = cz - 1; bz = chunkSize - 1; }
    else if (bz >= chunkSize) { nz = cz + 1; bz = 0; }

    if (nx < 0 || nx >= radius || nz < 0 || nz >= radius) return true;

    int neighborIdx = nx * radius + nz;

    if (!chunkDataList[neighborIdx]) return true;

    int finalIdx = bx + chunkSize * (by + chunkHeight * bz);
    return chunkDataList[neighborIdx][finalIdx] == BlockData::BlockType::AIR;
}