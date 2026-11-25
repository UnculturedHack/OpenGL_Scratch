#include <_gl.h>
#include <format>
#include <cassert>
#include <chrono>
#include <thread>
#include <utils.h>
#include "textures.h"
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
static constexpr const char* cpWoodTextFrgShaderSrcPath = "assets/shaders/woodTextureFrag.glsl";
static constexpr const char* cpWoodTextVtxShaderSrcPath = "assets/shaders/woodTextureVert.glsl";

static enum Project
{
    SIMPLE_1 = 0,
    SIMPLE_2 = 1,
    SIMPLE_3 = 2,
    SIMPLE_4 = 3,
    WOOD_RECT = 4,
};

static constexpr Project cProjectToRun = Project::WOOD_RECT;

static void processInput()
{

}

static void update(/*double aDeltaTimeMs*/)
{
    // logic
}

static void render(Project aProj, HDC* aphDc, const _gl::ShaderData aShaderData)
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
        case 4:
        {
            tx::renderWoodRect(aphDc, aShaderData);
            break;
        }
        default:
        {
            assert(false);
            break;
        }
    }
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

static void initShaderBuffers(Project aProj, uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
{
    switch (aProj)
    {
        case Project::SIMPLE_1:
        {
            tri::initSimpleBuffers(aVBO, aVAO);
            break;
        }
        case Project::SIMPLE_2:
        {
            tri::initSimple2Buffers(aVBO, aVAO, aEBO);
            break;
        }
        case Project::SIMPLE_3:
        {
            tri::initSimple3Buffers(aVBO, aVAO, aEBO);
            break;
        }
        case Project::SIMPLE_4:
        {
            tri::initSimple4Buffers(aVBO, aVAO);
            break;
        }
        case Project::WOOD_RECT:
        {
            tx::initWoodRect(aVBO, aVAO);
            break;
        }
        default:
        {
            assert(false);
            break;
        }
    }
}

static const _gl::ShaderData initShaders(Project aProj, const char* apVtxSrcPath, const char* apFrgSrcPath) //uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
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

static const _gl::ShaderData initProjectShaders(Project aProj)
{
    switch (aProj)
    {
        case Project::SIMPLE_1:
        {
            return initShaders(aProj, cpSimpleVtxShaderSrcPath, cpSimpleFrgShaderSrcPath);
        }
        case Project::SIMPLE_2:
        {
            return initShaders(aProj, cpSimpleVtxShader2SrcPath, cpSimpleFrgShader2SrcPath);
        }
        case Project::SIMPLE_3:
        {
            return initShaders(aProj, cpSimpleVtxShader3SrcPath, cpSimpleFrgShader3SrcPath);
        }
        case Project::SIMPLE_4:
        {
            return initShaders(aProj, cpSimpleVtxShader4SrcPath, cpSimpleFrgShader4SrcPath);
        }
        case Project::WOOD_RECT:
        {
            return initShaders(aProj, cpWoodTextVtxShaderSrcPath, cpWoodTextFrgShaderSrcPath);
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

    const _gl::ShaderData shaderData = initProjectShaders(cProjectToRun);
    if (!shaderData.m_cShaderProg)
    {
        utils::errMsg(g_wHndl, "Shader initialization failed.\n\nExiting...", "Shader Setup Error!");
        _gl::shutdownOpenGL(g_wHndl, g_dvcCtxt, g_rendCtxt);
        DestroyWindow(g_wHndl);
        return 1;
    }

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
        render(cProjectToRun, &g_dvcCtxt, shaderData);

        const chrono::duration frameElpsdTimePt = _clock::now() - frameStartTimePt;
        if (frameElpsdTimePt < cTargetFrameTime)
        {
            this_thread::sleep_for(cTargetFrameTime - frameElpsdTimePt);
        }
    }

    _gl::shutdownOpenGL(g_wHndl, g_dvcCtxt, g_rendCtxt);
    DestroyWindow(g_wHndl);
    return msg.wParam;
}