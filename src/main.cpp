#include "config.h"
#include "readShaderFile.hpp"
#include "assert.h"

// Render System
#include "render_system.hpp"

// Camera System
#include "camera_system.hpp"

const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;

struct CameraKeyboardInput{
    float forward = 0.f;
    float right = 0.f;
};

struct MouseState{
    bool initialized = false;
    double lastX = 0.0;
    double lastY = 0.0;
};

struct CameraMouseInput{ 
    double deltaX = 0.f;
    double deltaY = 0.f;
};

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
};

CameraKeyboardInput processInput (GLFWwindow* window, CameraSystem camera)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);

    // ======================================================================
    // =========================== Camera Control ===========================
    // ======================================================================
    CameraKeyboardInput cameraInput;

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraInput.forward += 1.f;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraInput.forward -= 1.f;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraInput.right += 1.f;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraInput.right -= 1.f;

    return cameraInput;
};

CameraMouseInput processMouseInput(GLFWwindow* window, MouseState& state)
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);

    CameraMouseInput input{};

    if (state.initialized) {
        input.deltaX = x - state.lastX;
        input.deltaY = y - state.lastY;
    }

    state.lastX = x;
    state.lastY = y;
    state.initialized = true;

    return input;
};

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
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    /* KNX : Init the renderer */
    renderer.init(window);

    /* KNX : Get Time */
    double previousTime = glfwGetTime();

    /* KNX : Remember the previous position */
    MouseState mouseState;

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* KNX : Calculate Delta Time */
        double currentTime = glfwGetTime();
        float deltaTime = static_cast<float>(currentTime - previousTime);
        previousTime = currentTime;

        /* KNX : Process User Input */
        CameraKeyboardInput cameraKeyboardInput = processInput(window, camera);
        CameraMouseInput cameraMouseInput = processMouseInput(window, mouseState);

        /* KNX : Camera Update */
        camera.update(cameraKeyboardInput.forward, cameraKeyboardInput.right,
            static_cast<float>(cameraMouseInput.deltaX), static_cast<float>(cameraMouseInput.deltaY), deltaTime);

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