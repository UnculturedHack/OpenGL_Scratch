#include <glad/gl.h>
#include <windows.h>
#include <string>
#include <format>
//#include <cstdio>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

using namespace std;

static GLFWwindow* s_pContextWindow = nullptr;
static bool s_glfwCallbacksInitialized = false;

static void printfDbg(const char* apMsgF, ...)
{
    va_list args;
    va_start(args, apMsgF);

    bool fmtSpecFound = false;
    constexpr char fmtSpecSymbol = '%';
    string fmtMsg = "";
    for(; *apMsgF; ++apMsgF)
    {
        const char c = *apMsgF;
        if (fmtSpecFound)
        {
            if (c == 'd')
            {
                int32_t d = va_arg(args, int32_t);
                fmtMsg.append(to_string(d));
            }
            else if (c == 'f')
            {
                double f = va_arg(args, double);
                fmtMsg.append(to_string(f));
            }
            else if (c == 'c')
            {
                int32_t cInt = va_arg(args, int32_t);
                fmtMsg.push_back(static_cast<char>(va_arg(args, int32_t)));
            }
            else if (c == 's')
            {
                const char* s = va_arg(args, const char*);
                fmtMsg.append(s);
            }
            else
            {
                fmtMsg.push_back(fmtSpecSymbol);
                if (c == fmtSpecSymbol)
                {
                    continue;
                }
                fmtMsg.push_back(c);
            }
            fmtSpecFound = false;
        }
        else
        {
            if (c == fmtSpecSymbol)
            {
                fmtSpecFound = true;
            }
            else
            {
                fmtMsg.push_back(c);
            }
        }
    }
    
    va_end(args);
    OutputDebugString(fmtMsg.append("\n").c_str());
}

static void handleFramebufferSizeChange(GLFWwindow*, int32_t aWidth, int32_t aHeight)
{
    glViewport(0, 0, aWidth, aHeight);
}

static void handleKeyPress(GLFWwindow* /*apWindow*/, int32_t aKey, int32_t aScancode, int32_t aAction, int32_t aMods)
{
    string action; // = nullptr;
    switch (aAction)
    {
        case GLFW_RELEASE:
        {
            action = "RELEASE";
            break;
        }
        case GLFW_PRESS:
        {
            action = "PRESS";
            break;
        }
        case GLFW_REPEAT:
        {
            action = "REPEAT";
            break;
        }
        default:
        {
            string tempActionStr = "#";
            action = tempActionStr.append(to_string(aAction)).c_str();
            break;
        }
    }

    string key;
    switch (aKey)
    {
        case GLFW_KEY_W:
        {
            key = "W";
            break;
        }
        case GLFW_KEY_A:
        {
            key = "A";
            break;
        }
        case GLFW_KEY_S:
        {
            key = "S";
            break;
        }
        case GLFW_KEY_D:
        {
            key = "D";
            break;
        }
        case GLFW_KEY_ESCAPE:
        {
            key = "ESC";
            glfwSetWindowShouldClose(s_pContextWindow, GLFW_TRUE);
            break;
        }
        default:
        {
            key = "#";
            key.append(to_string(aKey));
            break;
        }
    }

    printfDbg("key: %s | action: %s", key.c_str(), action.c_str());
}

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

void update(double /*aDeltaTimeMs*/)
{
    glfwPollEvents();
}

void renderFrame()
{
    glClear(GL_COLOR_BUFFER_BIT);



    glfwSwapBuffers(s_pContextWindow);
}

int32_t WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int32_t)
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

    initGlfwCallbacks();
    
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