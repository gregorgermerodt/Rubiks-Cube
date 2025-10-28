#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GameInterface.h"
#include "CubeLogic.h"

// glmw = Generic Library for Mathematics
// glm  = OpenGL Mathematics
// OpenGl = Open Graphics library
// GLSL = OpenGL Shading Language
// g_: global variables, m_: member variables, without prefix: local variables
// GLuint = OpenGL unsinged integer
GameInterface* g_myInterface; // for testing
GameInterface g_dummyInterface;
CubeLogic g_testCompound;

/**
* \brief Initializes the complete OpenGL stuff and returns a window.
* \return Opened window to paint into.
*/
GLFWwindow* InitializeSystem()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    GLFWwindow* window = glfwCreateWindow(1024, 768, "Rubix Cube", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glewExperimental = true;
    glewInit();

    g_myInterface->Initialize(window);

    return window;
}

/**
* \biref Runs the core loop of the game.
* \param The window to display our stuff in.
*/
void RunCoreLoop(GLFWwindow* window)
{
    // calculate deltaTime
    double lastTime = glfwGetTime();
    double timeDifference = 0.0; // zum debuggen auf fest auf z.b. 0.016 (16 ms) setzen

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        g_myInterface->Update(timeDifference);

        int screenWidth, screenHeight;
        glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
        float aspectRatio = static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
        glViewport(0, 0, screenWidth, screenHeight);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // fix for error when window is minimized
        int minimized = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
        if (!minimized)
        {
            g_myInterface->Render(aspectRatio);
        }
        
        glfwSwapBuffers(window);
        double currentTime = glfwGetTime();
        timeDifference = currentTime - lastTime;
        lastTime = currentTime;

    }
}

/**
* \brief Removes any left open resources.
*/
void ShutDownSystem()
{
    g_myInterface->ClearResources();
    glfwTerminate();
}

int main()
{
    g_myInterface = &g_testCompound;  // rotating cubies with different cubie colors

    GLFWwindow* window = InitializeSystem();
    RunCoreLoop(window);
    ShutDownSystem();
}
