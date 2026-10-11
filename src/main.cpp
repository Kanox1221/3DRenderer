#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"

// GLM include
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput (GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
}

void renderer_init (GLFWwindow* window)
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
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    /* KNX : Set given VBO as GL_ARRAY_BUFFER */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // GL_ARRAY_BUFFER = Buffer that stores vertex

    /* KNX : Copy the vertex data to buffer's memory */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* KNX : Create VAO */
    unsigned int VAO;
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
    unsigned int shaderProgram;
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
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    /* KNX : Send matrices to shader */
    int modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    int viewLoc = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

}

int main(int argc, char const *argv[])
{
    /* GLFW Window Initial Setup: https://www.glfw.org/documentation.html */
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit()){
        std::cerr << "[ERROR] GLFW : GLFW couldn't start" << std::endl;
        return -1;
    }
    /* KNX : Configure GLFW*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Renderer", NULL, NULL);

    if(!window){
        std::cerr << "[ERROR] GLFW : Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    std::cout << "[INFO] GLFW : Window Opened" << std::endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Load OpenGL functions through GLAD*/
	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
		std::cerr << "[ERROR] GLAD : Failed to initialize GLAD" << std::endl;
		return EXIT_FAILURE;
	}

    /* KNX : Tell OpenGL the size of the rendering window*/
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    /* KNX : Change the viewport size as user change the window size*/
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* KNX : Init the renderer*/
    renderer_init(window);

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* KNX : Process User Input */
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* KNX : Draw triangle */
        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}