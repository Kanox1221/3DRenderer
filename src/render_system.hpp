#pragma once

#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"

// GLM include
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class RenderSystem
{
public:
    bool init(GLFWwindow* window);
    void draw();
    void shutdown();
private:
    GLuint shaderProgram = 0;
    GLuint VAO = 0;
    GLuint VBO = 0;

    GLint modelLoc = -1;
    GLint viewLoc = -1;
    GLint projLoc = -1;

    std::vector<glm::mat4> models;
};