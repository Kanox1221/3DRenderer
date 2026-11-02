#include "render_system.hpp"

bool RenderSystem::init(GLFWwindow* window)
{


    // ======================================================================
    // =========================== Depth Test ===============================
    // ======================================================================
    glEnable(GL_DEPTH_TEST);

    // ======================================================================
    // ========== VBO & VAO & EBO setup for both cube and Ground ============
    // ======================================================================
    setupCube();
    setupGround();

    // ======================================================================
    // =============================== Shader ===============================
    // ======================================================================
    setupShaders();

    /* KNX : Get location for objectColor */
    objectColorLoc_ = glGetUniformLocation(shaderProgram_, "ObjectColor");

    // ======================================================================
    // ===================== Model & View & Projection ======================
    // ======================================================================
    /* KNX : Make mat4 for MVP */
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 3.0f),    // Camera Position
        glm::vec3(0.0f, 0.0f, 0.0f),    // Looking to
        glm::vec3(0.0f, 1.0f, 0.0f)     // Where is up
    );
    
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(
        glm::radians(45.0f),            // Fovy
        1920.0f / 1080.f,               // Aspect
        0.1f,                           // Near
        100.f                           // Far
    );
    
    /* KNX : Send matrices to shader */
    viewLoc_ = glGetUniformLocation(shaderProgram_, "view");

    projLoc_ = glGetUniformLocation(shaderProgram_, "projection");
    glUniformMatrix4fv(projLoc_, 1, GL_FALSE, glm::value_ptr(projection));

    modelLoc_ = glGetUniformLocation(shaderProgram_, "model");
    glUniformMatrix4fv(modelLoc_, 1, GL_FALSE, glm::value_ptr(model));

    return true;
}

void RenderSystem::setupShaders()
{
    /* KNX : Read our vertex shader file */
    std::string vertexShaderSource = readShaderFile(std::string(PROJECT_SOURCE_DIR) + "/shaders/triangle.vs.glsl");
    const char *vertexShaderSource_c = vertexShaderSource.c_str();

    /* KNX : Create and compile vertex shader*/
    unsigned int vertexShader; // Vertex shader ID
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource_c, NULL);
    glCompileShader(vertexShader);

    /* KNX : Error control for vertex shaders */
    GLint vs_compile_result;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vs_compile_result);
    if (vs_compile_result != GL_TRUE)
    {
        std::cerr << "[ERROR] Vertex Shader Compiler Error" << std::endl;

        char errBuff[1024];
        int bufLen;
        glGetShaderInfoLog(vertexShader, 1024, &bufLen, errBuff);
        std::cout << "Vertex Shader Info : " << errBuff << std::endl;
    }
    else
    {
        std::cout << "[INFO] Vertex Shader Compiled Successfully" << std::endl;
    }

    /* KNX : Repeat same thing for fragment shader */
    std::string fragmentShaderSource = readShaderFile(std::string(PROJECT_SOURCE_DIR) + "/shaders/triangle.fs.glsl");
    const char *fragmentShaderSource_c = fragmentShaderSource.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource_c, NULL);
    glCompileShader(fragmentShader);

    GLint fs_compile_result;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fs_compile_result);
    if (fs_compile_result != GL_TRUE)
    {
        std::cerr << "[ERROR] Fragment Shader Compiler Error" << std::endl;

        char errBuff[1024];
        int bufLen;
        glGetShaderInfoLog(fragmentShader, 1024, &bufLen, errBuff);
        std::cout << "Fragment Shader Info : " << errBuff << std::endl;
    }
    else
    {
        std::cout << "[INFO] Fragment Shader Compiled Successfully" << std::endl;
    }

    /* KNX : Create our shader program and link in shaders */
    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);

    GLint sp_link_result;
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &sp_link_result);
    if (sp_link_result != GL_TRUE)
    {
        std::cerr << "[ERROR] Shader Program Link Error" << std::endl;

        char errBuff[1024];
        int bufLen;
        glGetShaderInfoLog(shaderProgram_, 1024, &bufLen, errBuff);
        std::cout << "Shader Program Info : " << errBuff << std::endl;
    }
    else
    {
        std::cout << "[INFO] Shader Program Linked Successfully" << std::endl;
    }

    /* KNX : Activate the program*/
    glUseProgram(shaderProgram_);

    /* KNX : Delete shader after link
            Program already drawn the result when the link is successful*/
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void RenderSystem::setupGround()
{
    float ground_vertices[] = {
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
    };

    uint32_t ground_indices[] = {
        0, 2, 1,
        0, 3, 2,
    };

    /* VBO & VAO & EBO Setup for ground */
    glGenBuffers(1, &groundVBO_);
    glGenBuffers(1, &groundEBO_);
    glGenVertexArrays(1, &groundVAO_);

    // VAO should be bind before binding EBO
    // because EBO is binding to selected VAO
    glBindVertexArray(groundVAO_);

    glBindBuffer(GL_ARRAY_BUFFER, groundVBO_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, groundEBO_);

    uint32_t stride = 3 * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_indices), ground_indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);

    // Deselect VAO
    glBindVertexArray(0);
}

void RenderSystem::setupCube()
{
    float cube_vertices[] = {
        // Front Square
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,

        // Back Square
        -0.5f, -0.5f,  0.5f,
        0.5f, -0.5f,  0.5f,
        0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
    };

    uint32_t cube_indices[] = {
        // Front Square
        0, 2, 1,
        0, 3, 2,

        // Bottom Square
        0, 4, 5,
        0, 5, 1,

        // Back Square
        4, 5, 6,
        4, 6, 7,

        // Top Square
        3, 2, 6,
        3, 6, 7,

        // Left Square
        0, 7, 4,
        0, 3, 7,

        // Right Square
        1, 5, 6,
        1, 6, 2,
    };

    /* KNX : Create VBO */
    glGenBuffers(1, &cubeVBO_);

    /* KNX : Set given VBO as GL_ARRAY_BUFFER */
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO_); // GL_ARRAY_BUFFER = Buffer that stores vertex

    /* KNX : Copy the vertex data to buffer's memory */
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    /* KNX : Create VAO */
    glGenVertexArrays(1, &cubeVAO_);
    glBindVertexArray(cubeVAO_);

    /* KNX : VAO setting */
    unsigned int stride = 3 * sizeof(float);
    // Location = 0 / # of vertices / Type of data / Data to be normalized / Stride / Offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
    glEnableVertexAttribArray(0);

    /* KNX : Create EBO */
    glGenBuffers(1, &cubeEBO_);

    /* KNX : Set EBO as GL_ELEMENT_ARRAY_BUFFER */
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO_);

    /* KNX : Copy the indices data to buffer's memory */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void RenderSystem::draw(glm::mat4 view)
{
    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUniformMatrix4fv(viewLoc_, 1, GL_FALSE, glm::value_ptr(view));

    /* KNX : Draw Cube */
    glUniform3f(objectColorLoc_, 1.0f, 0.5f, 0.0f);     // Color cube as orange
    glBindVertexArray(cubeVAO_);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*) 0);
    // glDrawArrays(GL_TRIANGLES, 0, 3);

    /* KNX : Draw Floor */
    glUniform3f(objectColorLoc_, 0.4f, 0.4f, 0.4f);     // Color ground as grey
    glBindVertexArray(groundVAO_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);
}

void RenderSystem::shutdown()
{
    glDeleteBuffers(1, &cubeVBO_);
    glDeleteVertexArrays(1, &cubeVAO_);
    glDeleteProgram(shaderProgram_);

    cubeVBO_ = 0;
    cubeVAO_ = 0;
    shaderProgram_ = 0;
}
