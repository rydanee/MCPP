#ifndef MCPP_CHUNK_H
#define MCPP_CHUNK_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include "window/VAO.h"
#include "BlockData.hpp"
#include "glad/glad.h"

class Chunk {
    bool isMeshEmpty = true;
    int indexCount = 0;
    using ChunkDataDynamic = std::unique_ptr<BlockData::BlockType[]>;
public:
    Chunk(glm::vec3 pos, int chunkSize, int chunkHeight, ChunkDataDynamic data);

    void GenerateMesh(const std::vector<ChunkDataDynamic>& allChunkData, int idx, int radius);
    void UploadToGPU();

    void Render();
    void SetBlocks(ChunkDataDynamic data) { this->blocks = std::move(data); }
private:
    bool isNeighborEmpty(const std::vector<ChunkDataDynamic>& chunkDataList, int idx, int radius, int bx, int by, int bz);
    void IntegrateFace(BlockData::Faces face, glm::vec3 pos);
    void AddIndices(int count);

    glm::vec3 position;
    int chunkSize;
    int chunkHeight;
    int indCount;

    VAO* vao;
    ChunkDataDynamic blocks;

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> UV;
    std::vector<GLuint> indices;
};

#endif