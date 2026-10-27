#include "render_system.hpp"

bool RenderSystem::init(GLFWwindow* window)
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f,
    };

    // ======================================================================
    // ============================ VBO & VAO ===============================
    // ======================================================================
    /* KNX : Create VBO */
    glGenBuffers(1, &VBO);

    /* KNX : Set given VBO as GL_ARRAY_BUFFER */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // GL_ARRAY_BUFFER = Buffer that stores vertex

    /* KNX : Copy the vertex data to buffer's memory */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* KNX : Create VAO */
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    /* KNX : VAO setting */
    unsigned int stride = 3 * sizeof(float);
    // Location = 0 / # of vertices / Type of data / Data to be normalized / Stride / Offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0); 
    glEnableVertexAttribArray(0);

    // ======================================================================
    // =============================== Shader ===============================
    // ======================================================================
    /* KNX : Read our vertex shader file */
    std::string vertexShaderSource = readShaderFile(std::string(PROJECT_SOURCE_DIR) + "/shaders/triangle.vs.glsl");
    const char* vertexShaderSource_c = vertexShaderSource.c_str();

    /* KNX : Create and compile vertex shader*/
    unsigned int vertexShader;      // Vertex shader ID
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource_c, NULL);
    glCompileShader(vertexShader);

    /* KNX : Error control for vertex shaders */
    GLint vs_compile_result;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vs_compile_result);
    if (vs_compile_result != GL_TRUE){
        std::cerr << "[ERROR] Vertex Shader Compiler Error" << std::endl;

        char errBuff[1024];
        int bufLen;
        glGetShaderInfoLog(vertexShader, 1024, &bufLen, errBuff);
        std::cout << "Vertex Shader Info : " << errBuff << std::endl;
    } else {
        std::cout << "[INFO] Vertex Shader Compiled Successfully" << std::endl;
    }

    /* KNX : Repeat same thing for fragment shader */
    std::string fragmentShaderSource = readShaderFile(std::string(PROJECT_SOURCE_DIR) + "/shaders/triangle.fs.glsl");
    const char* fragmentShaderSource_c = fragmentShaderSource.c_str();

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource_c, NULL);
    glCompileShader(fragmentShader);

    GLint fs_compile_result;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fs_compile_result);
    if (fs_compile_result != GL_TRUE){
        std::cerr << "[ERROR] Fragment Shader Compiler Error" << std::endl;

        char errBuff[1024];
        int bufLen;
        glGetShaderInfoLog(fragmentShader, 1024, &bufLen, errBuff);
        std::cout << "Fragment Shader Info : " << errBuff << std::endl;
    } else {
        std::cout << "[INFO] Fragment Shader Compiled Successfully" << std::endl;
    }

    /* KNX : Create our shader program and link in shaders */
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint sp_link_result;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &sp_link_result);
    if (sp_link_result != GL_TRUE){
        std::cerr << "[ERROR] Shader Program Link Error" << std::endl;

        char errBuff[1024];
        int bufLen;
        glGetShaderInfoLog(shaderProgram, 1024, &bufLen, errBuff);
        std::cout << "Shader Program Info : " << errBuff << std::endl;
    } else {
        std::cout << "[INFO] Shader Program Linked Successfully" << std::endl;
    }

    /* KNX : Activate the program*/
    glUseProgram(shaderProgram);
    
    /* KNX : Delete shader after link
            Program already drawn the result when the link is successful*/
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ======================================================================
    // ===================== Model & View & Projection ======================
    // ======================================================================
    /* KNX : Make mat4 for MVP */
    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(
    //     model,
    //     glm::radians(45.0f),
    //     glm::vec3(0.0f, 1.0f, 0.0f)
    // );

    glm::mat4 leftModel = glm::mat4(1.0f);
    leftModel = glm::translate(
        leftModel, glm::vec3(-1.0f, 0.0f, 0.0f)
    );

    glm::mat4 rightModel = glm::mat4(1.0f);
    rightModel = glm::translate(
        rightModel, glm::vec3(1.0f, 0.0f, 0.0f)
    );

    models.push_back(model);
    models.push_back(leftModel);
    models.push_back(rightModel);

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
    viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    modelLoc = glGetUniformLocation(shaderProgram, "model");

    return true;
}

void RenderSystem::draw()
{
    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    /* KNX : Draw 3 triangles with 3 different model matrix */
    for (const glm::mat4& model : models){
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}

void RenderSystem::shutdown()
{
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

    VBO = 0;
    VAO = 0;
    shaderProgram = 0;
}
