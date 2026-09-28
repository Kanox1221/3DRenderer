#include "config.h"

int main(int argc, char const *argv[])
{
    /* GLFW Window Initial Setup: https://www.glfw.org/documentation.html */
    GLFWwindow* window;

    /* Initialize the library */
    if(!glfwInit()){
        std::cerr << "GLFW couldn't start" << std::endl;
        return -1;
    }
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1920, 1040, "3D Renderer", NULL, NULL);

    if(!window){
        glfwTerminate();
        return -1;
    }

    std::cout << "GLFW: Window Opened" << std::endl;

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        glfwTerminate();
		std::cerr << "GLAD couldn't start" << std::endl;
		return -1;
	}

    /* Loop until the user closes the window */
    while(!glfwWindowShouldClose(window)){
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}
