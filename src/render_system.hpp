#pragma once

#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"
#include "camera_system.hpp"

class RenderSystem
{
public:
    bool init(GLFWwindow* window);
    void draw(glm::mat4 view);
    void shutdown();
private:
    GLuint shaderProgram_ = 0;
    GLuint VAO_ = 0;
    GLuint VBO_ = 0;

    GLint modelLoc_ = -1;
    GLint viewLoc_ = -1;
    GLint projLoc_ = -1;

    std::vector<glm::mat4> models_;
};