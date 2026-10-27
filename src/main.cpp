#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"

// GLM include
#include <glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// Render System
#include "render_system.hpp"

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

int main(int argc, char const *argv[])
{
    RenderSystem renderer;

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
    renderer.init(window);

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* KNX : Process User Input */
        processInput(window);

        /* KNX : Draw */
        renderer.draw();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    renderer.shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}