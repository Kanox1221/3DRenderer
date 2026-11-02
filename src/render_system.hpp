#pragma once

#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"
#include "camera_system.hpp"

class RenderSystem
{
public:
    bool init(GLFWwindow *window);

    /* Setup helper functions */
    void setupShaders();
    void setupGround();
    void setupCube();

    void draw(glm::mat4 view);
    void shutdown();
private:
    GLuint shaderProgram_ = 0;
    GLuint objectColorLoc_ = 0;

    GLuint cubeVAO_ = 0;
    GLuint cubeVBO_ = 0;
    GLuint cubeEBO_ = 0;

    GLuint groundVAO_ = 0;
    GLuint groundVBO_ = 0;
    GLuint groundEBO_ = 0;

    GLint modelLoc_ = -1;
    GLint viewLoc_ = -1;
    GLint projLoc_ = -1;
};