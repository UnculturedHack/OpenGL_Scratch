#include <windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <string>
#include <format>

//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

using namespace std;

HWND g_wHndl = nullptr;
HDC g_devCtxt = nullptr;
HGLRC g_rendCtxt = nullptr;

static constexpr const char* cpWinClsName = "OpenGLWindow";
static constexpr int32_t cWinWidth = 1600;
static constexpr int32_t cWinHeight = 900;

//// Function pointer for wglCreateContextAttribsARB
//typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC hDC, HGLRC hShareContext, const int* attribList);

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

static void processInput()
{

}

static void update(/*double aDeltaTimeMs*/)
{
    // logic
}

static void render(HDC* aphDc)
{
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(*aphDc);
}

static LRESULT CALLBACK WndProc(HWND aHWnd, UINT aMsg, WPARAM aWParam, LPARAM aLParam)
{
    switch (aMsg)
    {
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_KEYDOWN:
        {
            if (aWParam == VK_ESCAPE)
            {
                PostQuitMessage(0);
            }
            break;
        }
        default:
        {
            return DefWindowProc(aHWnd, aMsg, aWParam, aLParam);
        }
    }
}

static void registerWndClass(HINSTANCE ahInstance)
{
    WNDCLASS wndCls{};
    wndCls.lpfnWndProc = WndProc;
    wndCls.hInstance = ahInstance;
    wndCls.style = CS_OWNDC;
    wndCls.lpszClassName = cpWinClsName;
    RegisterClass(&wndCls);
}

static void createWindow(HINSTANCE ahInstance)
{
    g_wHndl = CreateWindow(
        cpWinClsName, "Scratch", 
        WS_OVERLAPPEDWINDOW, 
        CW_USEDEFAULT, CW_USEDEFAULT, 
        cWinWidth, cWinHeight, nullptr, 
        nullptr, ahInstance, nullptr
    );
}

static void initOpenGL(HWND ahWnd, HDC* aphDc, HGLRC* aphGlrc)
{
    PIXELFORMATDESCRIPTOR pixFmtDesc =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32, // color depth
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0,
        24, // Depth buffer
        8, // Stencil buffer
        0, 0, 0, 0, 0, 0
    };

    *aphDc = GetDC(ahWnd);
    int32_t fmt = ChoosePixelFormat(*aphDc, &pixFmtDesc);
    SetPixelFormat(*aphDc, fmt, &pixFmtDesc);
    *aphGlrc = wglCreateContext(*aphDc);
    wglMakeCurrent(*aphDc, *aphGlrc);
}

static void shutdownOpenGL(HWND ahWnd, HDC ahDc, HGLRC ahGlrc)
{
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(ahGlrc);
    ReleaseDC(ahWnd, ahDc);
}

int32_t WINAPI wWinMain(_In_ HINSTANCE ahInstance, _In_opt_ HINSTANCE ahPrevInstance, _In_ PWSTR aArgsStr, _In_ int32_t anCmdShow)
{
    registerWndClass(ahInstance);
    createWindow(ahInstance);

    initOpenGL(g_wHndl, &g_devCtxt, &g_rendCtxt);
    ShowWindow(g_wHndl, anCmdShow);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        update();

        render(&g_devCtxt);
    }

    shutdownOpenGL(g_wHndl, g_devCtxt, g_rendCtxt);
    DestroyWindow(g_wHndl);
    return msg.wParam;
}


//===================GLFW=====================//

//static void handleFramebufferSizeChange(GLFWwindow*, int32_t aWidth, int32_t aHeight)
//{
//    glViewport(0, 0, aWidth, aHeight);
//}

//static void handleKeyPress(GLFWwindow* apWindow, int32_t aKey, int32_t aScancode, int32_t aAction, int32_t aMods)
//{
//    string action; // = nullptr;
//    switch (aAction)
//    {
//        case GLFW_RELEASE:
//        {
//            action = "RELEASE";
//            break;
//        }
//        case GLFW_PRESS:
//        {
//            action = "PRESS";
//            break;
//        }
//        case GLFW_REPEAT:
//        {
//            action = "REPEAT";
//            break;
//        }
//        default:
//        {
//            string tempActionStr = "#";
//            action = tempActionStr.append(to_string(aAction)).c_str();
//            break;
//        }
//    }
//
//    string key;
//    switch (aKey)
//    {
//        case GLFW_KEY_W:
//        {
//            key = "W";
//            break;
//        }
//        case GLFW_KEY_A:
//        {
//            key = "A";
//            break;
//        }
//        case GLFW_KEY_S:
//        {
//            key = "S";
//            break;
//        }
//        case GLFW_KEY_D:
//        {
//            key = "D";
//            break;
//        }
//        case GLFW_KEY_ESCAPE:
//        {
//            key = "ESC";
//            glfwSetWindowShouldClose(apWindow, GLFW_TRUE);
//            break;
//        }
//        default:
//        {
//            key = "#";
//            key.append(to_string(aKey));
//            break;
//        }
//    }
//
//    printfDbg("key: %s | action: %s", key.c_str(), action.c_str());
//}

//void initGlfwCallbacks(GLFWwindow* apWindow)
//{
//    //if (s_glfwCallbacksInitialized)
//    //{
//    //    printfDbg("Callbacks already initialized. Returning.");
//    //    return;
//    //}
//
//    glfwSetFramebufferSizeCallback(apWindow, handleFramebufferSizeChange);
//    //glfwSetKeyCallback(s_pContextWindow, handleKeyPress);
//    //s_glfwCallbacksInitialized = true;
//}

//int32_t WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ PWSTR, _In_ int32_t)
//{
    //if (glfwInit() != GLFW_TRUE)
    //{
    //    return 1;
    //}

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //GLFWwindow* pWindow = glfwCreateWindow(1600, 900, "Scratch", nullptr, nullptr);
    //if (!pWindow)
    //{
    //    printfDbg("Failed to create window. Exiting...\n");
    //    glfwTerminate();
    //    return 1;
    //}

    //glfwMakeContextCurrent(pWindow);

    //if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(glfwGetProcAddress)))
    //{
    //    printfDbg("Failed to load available OpenGL functions. Exiting...");
    //    glfwDestroyWindow(pWindow);
    //    glfwTerminate();
    //}

    //initGlfwCallbacks(pWindow);
    //
    //glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    //double previousFrameTimeS = glfwGetTime();
    //while (!glfwWindowShouldClose(pWindow))
    //{
    //    double currentFrameTimeS = glfwGetTime();
    //    constexpr double cMsPerSecond = 1000;
    //    double deltaTimeMs = (currentFrameTimeS - previousFrameTimeS) * cMsPerSecond;
    //    update(deltaTimeMs);
    //    renderFrame();
    //    glfwPollEvents();
    //}

    //glfwTerminate();
//
//    return 0;
//}

//void renderFrame(GLFWwindow* pWindow)
//{
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    
//
//    glfwSwapBuffers(pWindow);
//}//}