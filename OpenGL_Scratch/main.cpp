#include <_gl.h>
#include <format>
#include <cassert>
#include <chrono>
#include <thread>
#include <utils.h>
#include "triangles.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

using _clock = chrono::steady_clock;
const chrono::duration cTargetFrameTime = chrono::duration<double>(1.0 / 5.0);

HWND g_wHndl = nullptr;
HDC g_dvcCtxt = nullptr;
HGLRC g_rendCtxt = nullptr;

static constexpr const char* cpWinClsName = "OpenGLWindow";
static constexpr int32_t cWinWidth = 1600;
static constexpr int32_t cWinHeight = 900;
static constexpr int32_t cMaxInfoLogLength = 512;

static constexpr const char* cpSimpleVtxShaderSrcPath = "assets/shaders/simpleVert.glsl";
static constexpr const char* cpSimpleFrgShaderSrcPath = "assets/shaders/simpleFrag.glsl";
static constexpr const char* cpSimpleVtxShader2SrcPath = "assets/shaders/simpleVert2.glsl";
static constexpr const char* cpSimpleFrgShader2SrcPath = "assets/shaders/simpleFrag2.glsl";
static constexpr const char* cpSimpleVtxShader3SrcPath = "assets/shaders/simpleVert3.glsl";
static constexpr const char* cpSimpleFrgShader3SrcPath = "assets/shaders/simpleFrag3.glsl";
static constexpr const char* cpSimpleVtxShader4SrcPath = "assets/shaders/simpleVert4.glsl";
static constexpr const char* cpSimpleFrgShader4SrcPath = "assets/shaders/simpleFrag4.glsl";

static void processInput()
{

}

static void update(/*double aDeltaTimeMs*/)
{
    // logic
}

static void render(uint32_t aProj, HDC* aphDc, const _gl::ShaderData aShaderData)
{
    switch (aProj)
    {
        case 0:
        {
            tri::renderSimple(aphDc, aShaderData);
            break;
        }
        case 1:
        {
            tri::renderSimple2(aphDc, aShaderData);
            break;
        }
        case 2:
        {
            tri::renderSimple3(aphDc, aShaderData);
            break;
        }
        case 3:
        {
            tri::renderSimple4(aphDc, aShaderData);
            break;
        }
    }

    //glClear(GL_COLOR_BUFFER_BIT);

    //-------------------------------------------------------
    //_gl::glUseProgram(aShaderData.m_cShaderProg);
    //int32_t sharedColorIndex = _gl::glGetUniformLocation(aShaderData.m_cShaderProg, "sharedColor");
    //if (sharedColorIndex >= 0)
    //{
    //    SYSTEMTIME now;
    //    GetLocalTime(&now);
        //float redVal = (sin(now.wMilliseconds) / 2.0f) + 0.5f;
    //    float grnVal = sin(now.wMilliseconds) / 2.0f;
    //    // glUseProgram() must be called before a uniform is updated.
    //    _gl::glUniform4f(sharedColorIndex, redVal, grnVal, 0.0f, 1.0f);
    //}

    //_gl::glBindVertexArray(aShaderData.m_cVtxArryObj);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //-------------------------------------------------------

    //_gl::glBindVertexArray(aShaderData.m_cVtxArryObj);
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    
    //SwapBuffers(*aphDc);
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
        case WM_SIZE:
        {
            const int32_t cWidth = LOWORD(aLParam);
            const int32_t cHeight = HIWORD(aLParam);
            glViewport(0, 0, cWidth, cHeight);
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

static bool createEmptyShaders(uint32_t& aVertexShader, uint32_t& aFragmentShader)
{
    aVertexShader = _gl::glCreateShader(_gl::GL_VERTEX_SHADER);
    if (!aVertexShader)
    {
        utils::errMsg(g_wHndl, "Failed to create vertex shader!", "Shader Creation Failed!\n\nExiting...");
        return false;
    }

    aFragmentShader = _gl::glCreateShader(_gl::GL_FRAGMENT_SHADER);
    if (!aFragmentShader)
    {
        utils::errMsg(g_wHndl, "Failed to create fragment shader!", "Shader Creation Failed!\n\nExiting...");
        return false;
    }

    return true;
}

static bool compileShaders(uint32_t aVertexShader, const _gl::GLchar* apVertShaderSrc, uint32_t aFragmentShader, const _gl::GLchar* apFragShaderSrc)
{
    _gl::glShaderSource(aVertexShader, 1, &apVertShaderSrc, nullptr);
    _gl::glCompileShader(aVertexShader);
    int32_t vtxCompStatus;
    _gl::glGetShaderiv(aVertexShader, _gl::GL_COMPILE_STATUS, &vtxCompStatus);
    if (!vtxCompStatus)
    {
        char infoLog[cMaxInfoLogLength]{};
        _gl::glGetShaderInfoLog(aVertexShader, cMaxInfoLogLength, nullptr, infoLog);
        utils::errMsg(g_wHndl, format("Vertex shader compilation failed! Log:\n\n{}", infoLog).c_str(), "Vert Shader Comp Failed!");
        return false;
    }

    _gl::glShaderSource(aFragmentShader, 1, &apFragShaderSrc, nullptr);
    _gl::glCompileShader(aFragmentShader);
    int32_t frgCompStatus;
    _gl::glGetShaderiv(aFragmentShader, _gl::GL_COMPILE_STATUS, &frgCompStatus);
    if (!frgCompStatus)
    {
        char infoLog[cMaxInfoLogLength]{};
        _gl::glGetShaderInfoLog(aFragmentShader, cMaxInfoLogLength, nullptr, infoLog);
        utils::errMsg(g_wHndl, format("Fragment shader compilation failed! Log:\n\n{}", infoLog).c_str(), "Frag Shader Comp Failed!");
        return false;
    }

    return true;
}

static uint32_t linkShaderProgram(uint32_t& aVertexShader, uint32_t& aFragmentShader)
{
    uint32_t shaderProg = _gl::glCreateProgram();
    if (!shaderProg)
    {
        utils::errMsg(g_wHndl, "Failed to create shader program!", "Error!");
        return 0;
    }

    _gl::glAttachShader(shaderProg, aVertexShader);
    _gl::glAttachShader(shaderProg, aFragmentShader);
    _gl::glLinkProgram(shaderProg);
    int32_t linkStatus;
    _gl::glGetProgramiv(shaderProg, _gl::GL_LINK_STATUS, &linkStatus);
    if (!linkStatus)
    {
        char infoLog[cMaxInfoLogLength]{};
        _gl::glGetProgramInfoLog(shaderProg, cMaxInfoLogLength, nullptr, infoLog);
        utils::errMsg(g_wHndl, format("Failed to link the shader program. Program log: {}", infoLog).c_str(), "Link Error!");
        _gl::glDeleteProgram(shaderProg);
        return 0;
    }

    return shaderProg;
}

static void initShaderBuffers(uint32_t aProj, uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
{
    switch (aProj)
    {
        case 0:
        {
            tri::initSimpleBuffers(aVBO, aVAO);
            break;
        }
        case 1:
        {
            tri::initSimple2Buffers(aVBO, aVAO, aEBO);
            break;
        }
        case 2:
        {
            tri::initSimple3Buffers(aVBO, aVAO, aEBO);
            break;
        }
        case 3:
        {
            tri::initSimple4Buffers(aVBO, aVAO);
            break;
        }
    }
}

//static void initShaderBuffers(uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
//{
//    //float vertices[] =
//    //{
//    //     0.5f,  0.5f, 0.0f,
//    //     0.5f, -0.5f, 0.0f,
//    //    -0.5f, -0.5f, 0.0f,
//    //    -0.5f,  0.5f, 0.0f
//    //};
//
//    //float vertices[] = {
//    //-0.5f, -0.5f, 0.0f,
//    // 0.5f, -0.5f, 0.0f,
//    // 0.0f,  0.5f, 0.0f
//    //};
//
//    //float vertices[] = {
//    //    // Tri 1
//    //    -0.5f, -0.5f, 0.0f,
//    //    0.0f, -0.5f, 0.0f,
//    //    -0.25f, 0.0f, 0.0f,
//    //    // Tri 2
//    //    0.5f, 0.5f, 0.0f,
//    //    0.25f,  0.0f, 0.0f,
//    //    0.0f, 0.5f, 0.0f
//    //};
//
//    
//
//    //uint32_t indices[] =
//    //{
//    //    0,1,2,3,4,5
//    //    //0, 1, 3, 5, 2, 4
//    //    //1, 2, 3
//    //};
//
//    _gl::glGenVertexArrays(1, &aVAO);
//    _gl::glGenBuffers(1,  &aVBO);
//    //_gl::glGenBuffers(1, &aEBO);
//    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
//    _gl::glBindVertexArray(aVAO);
//
//    _gl::glBindBuffer(_gl::GL_ARRAY_BUFFER, aVBO);
//    _gl::glBufferData(_gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, _gl::GL_STATIC_DRAW);
//
//    //_gl::glBindBuffer(_gl::GL_ELEMENT_ARRAY_BUFFER, aEBO);
//    //_gl::glBufferData(_gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, _gl::GL_STATIC_DRAW);
//
//    // position attribute
//    _gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    _gl::glEnableVertexAttribArray(0);
//
//    // color attribute
//    _gl::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//    _gl::glEnableVertexAttribArray(1);
//
//    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
//    //_gl::glBindBuffer(_gl::GL_ARRAY_BUFFER, 0);
//
//    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
//    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
//    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
//    //_gl::glBindVertexArray(0);
//}

static const _gl::ShaderData initShaders(uint32_t aProj, const char* apVtxSrcPath, const char* apFrgSrcPath) //uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
{
    string vtxSrc;
    string frgSrc;
    if (!_gl::loadShaderSrc(apVtxSrcPath, vtxSrc) || !_gl::loadShaderSrc(apFrgSrcPath, frgSrc))
    {
        return {};
    }

    uint32_t vertexShader;
    uint32_t fragmentShader;
    if (!createEmptyShaders(vertexShader, fragmentShader))
    {
        return {};
    }

    if (!compileShaders(vertexShader, vtxSrc.c_str(), fragmentShader, frgSrc.c_str()))
    {
        return {};
    }

    uint32_t shaderProg = linkShaderProgram(vertexShader, fragmentShader);
    _gl::glDeleteShader(vertexShader);
    _gl::glDeleteShader(fragmentShader);

    if (!shaderProg)
    {
        return {};
    }

    uint32_t VBO;
    uint32_t VAO;
    uint32_t EBO;

    initShaderBuffers(aProj, VBO, VAO, EBO);

    if (!VBO || !VAO || !EBO)
    {
        return {};
    }

    return { shaderProg, VBO, VAO, EBO };
}

static const _gl::ShaderData initProjectShaders(uint32_t aProj)
{
    switch (aProj)
    {
        case 0:
        {
            return initShaders(aProj, cpSimpleVtxShaderSrcPath, cpSimpleFrgShaderSrcPath);
        }
        case 1:
        {
            return initShaders(aProj, cpSimpleVtxShader2SrcPath, cpSimpleFrgShader2SrcPath);
        }
        case 2:
        {
            return initShaders(aProj, cpSimpleVtxShader3SrcPath, cpSimpleFrgShader3SrcPath);
        }
        case 3:
        {
            return initShaders(aProj, cpSimpleVtxShader4SrcPath, cpSimpleFrgShader4SrcPath);
        }
        default:
        {
            assert(false);
            break;
        }
    }
}

int32_t WINAPI wWinMain(_In_ HINSTANCE ahInstance, _In_opt_ HINSTANCE ahPrevInstance, _In_ PWSTR aArgsStr, _In_ int32_t anCmdShow)
{
    registerWndClass(ahInstance);
    createWindow(ahInstance);

    if (!_gl::initOpenGL(g_wHndl, &g_dvcCtxt, &g_rendCtxt, cWinWidth, cWinHeight))
    {
        utils::errMsg(g_wHndl, "Could not create an OpenGL 3.3 context.\n\nExiting...", "OpenGL Init Error!");
        DestroyWindow(g_wHndl);
        return 1;
    }

    constexpr uint32_t proj = 3;

    const _gl::ShaderData shaderData = initProjectShaders(proj);
    if (!shaderData.m_cShaderProg)
    {
        utils::errMsg(g_wHndl, "Shader initialization failed.\n\nExiting...", "Shader Setup Error!");
        _gl::shutdownOpenGL(g_wHndl, g_dvcCtxt, g_rendCtxt);
        DestroyWindow(g_wHndl);
        return 1;
    }

    //-----------
    //int32_t maxVtxAttbs;
    //glGetIntegerv(_gl::GL_MAX_VERTEX_ATTRIBS, &maxVtxAttbs); = 16 for my desktop
    //-----------

    _gl::glUseProgram(shaderData.m_cShaderProg);
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    ShowWindow(g_wHndl, anCmdShow);

    MSG msg;
    bool shutdownRequested = false;
    while (!shutdownRequested)
    {
        const _clock::time_point frameStartTimePt = _clock::now();
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                shutdownRequested = true;
                break;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (shutdownRequested)
        {
            break;
        }

        processInput();
        update();
        render(proj, &g_dvcCtxt, shaderData);

        const chrono::duration frameElpsdTimePt = _clock::now() - frameStartTimePt;
        if (frameElpsdTimePt < cTargetFrameTime)
        {
            this_thread::sleep_for(cTargetFrameTime - frameElpsdTimePt);
        }
    }

    // This while loop does not run continuously; GetMessage is a blocking call that waits until a new message is received.
    //while (GetMessage(&msg, nullptr, 0, 0))
    //{
    //    TranslateMessage(&msg);
    //    DispatchMessage(&msg);

    //    processInput();
    //    update();
    //    render(&g_dvcCtxt, shaderData);
    //}

    _gl::shutdownOpenGL(g_wHndl, g_dvcCtxt, g_rendCtxt);
    DestroyWindow(g_wHndl);
    return msg.wParam;
}