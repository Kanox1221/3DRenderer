#pragma once

#include "config.h"

struct VertexData {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
};

struct MeshData {
    std::vector<VertexData> vertices;
    std::vector<unsigned int> indices;
};