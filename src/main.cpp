#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"

// Render System
#include "render_system.hpp"

// Camera System
#include "camera_system.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

struct CameraInput{
    float forward = 0.f;
    float right = 0.f;
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

CameraInput processInput (GLFWwindow* window, CameraSystem camera)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);

    // ======================================================================
    // =========================== Camera Control ===========================
    // ======================================================================
    CameraInput cameraInput;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraInput.forward += 1.f;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraInput.forward -= 1.f;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraInput.right -= 1.f;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraInput.right += 1.f;

    return cameraInput;
}

int main(int argc, char const *argv[])
{
    RenderSystem renderer;
    CameraSystem camera;

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

    double previousTime = glfwGetTime();

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* KNX : Calculate Delta Time */
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - previousTime);
        previousTime = currentTime;

        /* KNX : Process User Input */
        CameraInput cameraInput = processInput(window, camera);

        /* KNX : Camera Update */
        camera.update(cameraInput.forward, cameraInput.right, deltaTime);

        /* KNX : Draw */
        renderer.draw(camera.getViewMatrix());

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