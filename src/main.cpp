#include "config.h"
#include "readShaderFile.hpp"

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

    /* KNX : Create VBO */
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    /* KNX : Set given VBO as GL_ARRAY_BUFFER */
    glBindBuffer(GL_ARRAY_BUFFER, VBO);     // GL_ARRAY_BUFFER = Buffer that stores vertex

    /* KNX : Copy the vertex data to buffer's memory */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    /* KNX : Read our shader file */
    std::string vertexShaderSource = readShaderFile(std::string(PROJECT_SOURCE_DIR) + "/shaders/triangle.vs.glsl");
    const char* vertexShaderSource_c = vertexShaderSource.c_str();
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

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* KNX : Process User Input */
        processInput(window);

        /* Render here */
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}