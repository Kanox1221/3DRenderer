#pragma once

#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"
#include "camera_system.hpp"
#include "mesh_data.hpp"
#include "stb_image/stb_image.h"

class RenderSystem
{
public:
    bool init(GLFWwindow *window, const std::vector<MeshData>& meshes);

    /* Setup helper functions */
    void setupShaders();
    void setupGround();
    void setupCube();
    void setupMesh(const MeshData& mesh);

    void draw(glm::mat4 view);
    void shutdown();
private:
    GLuint shaderProgram_ = 0;

    GLuint cubeVAO_ = 0;
    GLuint cubeVBO_ = 0;
    GLuint cubeEBO_ = 0;

    GLuint groundVAO_ = 0;
    GLuint groundVBO_ = 0;
    GLuint groundEBO_ = 0;

    GLuint meshVAO_ = 0;
    GLuint meshVBO_ = 0;
    GLuint meshEBO_ = 0;
    uint32_t numIndices_ = 0;

    GLuint grassTexture_ = 0;

    GLint objectColorLoc_ = -1;
    GLint showUVLoc_ = -1;
    GLint useTextureLoc_ = -1;

    GLint modelLoc_ = -1;
    GLint viewLoc_ = -1;
    GLint projLoc_ = -1;

    std::vector<glm::mat4> models_;
};