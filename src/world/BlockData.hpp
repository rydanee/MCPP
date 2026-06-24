#ifndef MCPP_BLOCKDATA_HPP
#define MCPP_BLOCKDATA_HPP
#include <vector>
#include <glm/glm.hpp>

class BlockData {
public:
    enum BlockType {
        AIR=0,
        DIRT=1,
        GRASS=2,
        STONE=3
    };
    enum Faces {
        FRONT=0,
        BACK=1,
        RIGHT=2,
        LEFT=3,
        TOP=4,
        BOTTOM=5
    };

    static inline std::vector<std::vector<glm::vec3>> def_vertices {
        // FRONT Face (Facing +Z) -> Triggered by z + 1 checks
        std::vector<glm::vec3>{
            glm::vec3(-0.5f,  0.5f,  0.5f),
            glm::vec3( 0.5f,  0.5f,  0.5f),
            glm::vec3( 0.5f, -0.5f,  0.5f),
            glm::vec3(-0.5f, -0.5f,  0.5f)
        },
        // BACK Face (Facing -Z) -> Triggered by z - 1 checks
        std::vector<glm::vec3> {
            glm::vec3( 0.5f,  0.5f, -0.5f),
            glm::vec3(-0.5f,  0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, -0.5f),
            glm::vec3( 0.5f, -0.5f, -0.5f)
        },
        // RIGHT Face (Facing +X) -> Triggered by x + 1 checks
        std::vector<glm::vec3> {
            glm::vec3( 0.5f,  0.5f,  0.5f),
            glm::vec3( 0.5f,  0.5f, -0.5f),
            glm::vec3( 0.5f, -0.5f, -0.5f),
            glm::vec3( 0.5f, -0.5f,  0.5f)
        },
        // LEFT Face (Facing -X) -> Triggered by x - 1 checks
        std::vector<glm::vec3> {
            glm::vec3(-0.5f,  0.5f, -0.5f),
            glm::vec3(-0.5f,  0.5f,  0.5f),
            glm::vec3(-0.5f, -0.5f,  0.5f),
            glm::vec3(-0.5f, -0.5f, -0.5f)
        },
        // TOP Face (Facing +Y) -> Triggered by y + 1 checks
        std::vector<glm::vec3> {
            glm::vec3(-0.5f,  0.5f, -0.5f),
            glm::vec3( 0.5f,  0.5f, -0.5f),
            glm::vec3( 0.5f,  0.5f,  0.5f),
            glm::vec3(-0.5f,  0.5f,  0.5f)
        },
        // BOTTOM Face (Facing -Y) -> Triggered by y - 1 checks
        std::vector<glm::vec3> {
            glm::vec3(-0.5f, -0.5f,  0.5f),
            glm::vec3( 0.5f, -0.5f,  0.5f),
            glm::vec3( 0.5f, -0.5f, -0.5f),
            glm::vec3(-0.5f, -0.5f, -0.5f)
        }
    };

    static inline std::vector<glm::vec2> def_UV {
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 0.0f)
    };
};

#endif
