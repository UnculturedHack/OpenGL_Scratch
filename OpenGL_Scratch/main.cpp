#include "include/_gl.h"
#include <gl/GLU.h>
#include <string>
//#include <format>

//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

using namespace std;

HWND g_wHndl = nullptr;
HDC g_dvcCtxt = nullptr;
HGLRC g_rendCtxt = nullptr;

static constexpr const char* cpWinClsName = "OpenGLWindow";
static constexpr int32_t cWinWidth = 1600;
static constexpr int32_t cWinHeight = 900;

// Function pointer for wglCreateContextAttribsARB
//using PFNWGLCREATECONTEXTATTRIBSARBPROC = HGLRC(WINAPI*)(HDC, HGLRC, const int*);
//using PFNGLGENBUFFERSPROC = void (APIENTRY*)(GLsizei, GLuint*);
//using PFNGLBINDBUFFERPROC = void (APIENTRY*)(GLenum, GLuint);

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

static void errMsg(const char* const apText, const char* const apCaption)
{
    MessageBox(g_wHndl, apText, apCaption, MB_OK | MB_ICONERROR);
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

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };
    
    uint32_t VBO;
    _gl::glGenBuffers(1, &VBO);                                       
    _gl::glBindBuffer(_gl::GL_ARRAY_BUFFER, VBO);
    _gl::glBufferData(_gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, _gl::GL_STATIC_DRAW);

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

static bool initOpenGL(HWND ahWnd, HDC* aphDc, HGLRC* aphGlrc)
{
    if (!_gl::load())
    {
        errMsg("Failed to load OpenGL functions!", "OpenGL Load Failed!");
        return false;
    }

    PIXELFORMATDESCRIPTOR pixFmtDesc{}; 
    pixFmtDesc.nSize = sizeof(pixFmtDesc);
    pixFmtDesc.nVersion = 1;
    pixFmtDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pixFmtDesc.iPixelType = PFD_TYPE_RGBA;
    pixFmtDesc.cColorBits = 32;
    pixFmtDesc.cDepthBits = 24;
    pixFmtDesc.cStencilBits = 8;
    pixFmtDesc.iLayerType = PFD_MAIN_PLANE;
    
    //=
    //{
    //    sizeof(PIXELFORMATDESCRIPTOR),
    //    1,
    //    PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
    //    PFD_TYPE_RGBA,
    //    32, // color depth
    //    0, 0, 0, 0, 0, 0, 0, 0,
    //    0, 0, 0, 0, 0,
    //    24, // Depth buffer
    //    8, // Stencil buffer
    //    0, 0, 0, 0, 0, 0
    //};

    *aphDc = GetDC(ahWnd);
    if (!*aphDc)
    {
        return false;
    }

    const int32_t pixFmt = ChoosePixelFormat(*aphDc, &pixFmtDesc);
    if (pixFmt == 0 || !SetPixelFormat(*aphDc, pixFmt, &pixFmtDesc))
    {
        //ReleaseDC(ahWnd, *aphDc);
        //*aphDc = nullptr;
        return false;
    }

    HGLRC tempGlrc = wglCreateContext(*aphDc);
    if (!tempGlrc || !wglMakeCurrent(*aphDc, tempGlrc))
    {
        if (tempGlrc)
        {
            wglDeleteContext(tempGlrc);
        }

        //ReleaseDC
        return false;
    }

    //PFNWGLCREATECONTEXTATTRIBSARBPROC createContextAttribs = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(
    //        wglGetProcAddress("wglCreateContextAttribsARB"));
    //if (!createContextAttribs)
    //{
    //    //wglMakeCurrent(nullptr, nullptr);
    //    if (tempGlrc)
    //    {
    //        wglDeleteContext(tempGlrc);
    //    }
    //    return false;
    //}

    constexpr int WGL_CONTEXT_MAJOR_VERSION_ARB = 0x2091;
    constexpr int WGL_CONTEXT_MINOR_VERSION_ARB = 0x2092;
    constexpr int WGL_CONTEXT_PROFILE_MASK_ARB = 0x9126;
    constexpr int WGL_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001;

    const int32_t cAttribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };


    *aphGlrc = _gl::createContextAttribs(*aphDc, *aphGlrc, cAttribs); // createContextAttribs(*aphDc, nullptr, cAttribs); // wglCreateContext(*aphDc);
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(tempGlrc);

    if (!*aphGlrc || !wglMakeCurrent(*aphDc, *aphGlrc))
    {
        if (*aphGlrc)
        {
            wglDeleteContext(*aphGlrc);
        }

        return false;
    }

    glViewport(0, 0, cWinWidth, cWinHeight);
    return true;
}

static void shutdownOpenGL(HWND ahWnd, HDC& ahDc, HGLRC& ahGlrc)
{
    wglMakeCurrent(nullptr, nullptr);

    if (ahGlrc)
    {
        wglDeleteContext(ahGlrc);
        ahGlrc = nullptr;
    }

    if (ahDc)
    {
        ReleaseDC(ahWnd, ahDc);
        ahDc = nullptr;
    }
}

int32_t WINAPI wWinMain(_In_ HINSTANCE ahInstance, _In_opt_ HINSTANCE ahPrevInstance, _In_ PWSTR aArgsStr, _In_ int32_t anCmdShow)
{
    registerWndClass(ahInstance);
    createWindow(ahInstance);

    if (!initOpenGL(g_wHndl, &g_dvcCtxt, &g_rendCtxt))
    {
        //MessageBox(g_wHndl, "Could not create an OpenGL 3.3 context.", "OpenGL Init Error", MB_OK | MB_ICONERROR);
        errMsg("Could not create an OpenGL 3.3 context.", "OpenGL Init Error");
        DestroyWindow(g_wHndl);
        return 1;
    }

    ShowWindow(g_wHndl, anCmdShow);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        update();

        render(&g_dvcCtxt);
    }

    shutdownOpenGL(g_wHndl, g_dvcCtxt, g_rendCtxt);
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