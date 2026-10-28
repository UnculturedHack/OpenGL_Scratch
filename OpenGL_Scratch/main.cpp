#include <glad/gl.h>
#include <cstdio>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace std;

static GLFWwindow* s_pContextWindow = nullptr;

void handleFramebufferSizeChange(GLFWwindow*, int32_t width, int32_t height)
{
    glViewport(0, 0, width, height);
}

void handleKeyPress(GLFWwindow* /*apWindow*/, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    switch (key)
    {
        case GLFW_KEY_W:
        {
            break;
        }
        case GLFW_KEY_A:
        {
            break;
        }
        case GLFW_KEY_S:
        {
            break;
        }
        case GLFW_KEY_D:
        {
            break;
        }
        case GLFW_KEY_ESCAPE:
        {
            glfwSetWindowShouldClose(s_pContextWindow, GLFW_TRUE);
            break;
        }
    }
}

static bool s_glfwCallbacksInitialized = false;

void initGlfwCallbacks()
{
    if (s_glfwCallbacksInitialized)
    {
        printf("Callbacks already initialized. Returning.");
        return;
    }

    glfwSetFramebufferSizeCallback(s_pContextWindow, handleFramebufferSizeChange);
    glfwSetKeyCallback(s_pContextWindow, handleKeyPress);
    s_glfwCallbacksInitialized = true;
}

void update(double aDeltaTimeMs)
{
    
}

void renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);



    glfwSwapBuffers(s_pContextWindow);
}

int32_t main()
{
    if (glfwInit() != GLFW_TRUE)
    {
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    s_pContextWindow = glfwCreateWindow(1600, 900, "Scratch", nullptr, nullptr);
    if (!s_pContextWindow)
    {
        printf("Failed to create window. Exiting...\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(s_pContextWindow);

    if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    {
        printf("Failed to load available OpenGL functions. Exiting...");
        glfwDestroyWindow(s_pContextWindow);
        glfwTerminate();
    }

    
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    double previousFrameTimeS = glfwGetTime();
    while (!glfwWindowShouldClose(s_pContextWindow))
    {
        double currentFrameTimeS = glfwGetTime();
        constexpr double cMsPerSecond = 1000;
        double deltaTimeMs = (currentFrameTimeS - previousFrameTimeS) * cMsPerSecond;
        update(deltaTimeMs);
        renderFrame();
    }

    glfwTerminate();
    return 0;
}