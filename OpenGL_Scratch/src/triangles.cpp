#include "triangles.h"
#include <_gl.h>
#include <cmath>
#include <stdexcept>

void tri::renderSimple(HDC* aphDc, const _gl::ShaderData aShaderData)
{
    throw std::runtime_error("renderSimple() function not implemented!");
}

void tri::initSimpleBuffers(uint32_t& aVBO, uint32_t& aVAO)
{
    throw std::runtime_error("initSimpleBuffers() function not implemented!");
}

void tri::renderSimple2(HDC* aphDc, const _gl::ShaderData aShaderData)
{
    throw std::runtime_error("renderSimple2() function not implemented!");
}

void tri::initSimple2Buffers(uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
{
    throw std::runtime_error("initSimple2Buffers() function not implemented!");
}

void tri::renderSimple3(HDC* aphDc, const _gl::ShaderData aShaderData)
{
    glClear(GL_COLOR_BUFFER_BIT);

    _gl::glUseProgram(aShaderData.m_cShaderProg);
    int32_t sharedColorIndex = _gl::glGetUniformLocation(aShaderData.m_cShaderProg, "sharedColor");
    if (sharedColorIndex >= 0)
    {
        SYSTEMTIME now;
        GetLocalTime(&now);
        float redVal = (sin(now.wMilliseconds) / 2.0f) + 0.5f;
        float grnVal = sin(now.wMilliseconds) / 2.0f;
        // glUseProgram() must be called before a uniform is updated.
        _gl::glUniform4f(sharedColorIndex, redVal, grnVal, 0.0f, 1.0f);
    }

    _gl::glBindVertexArray(aShaderData.m_cVtxArryObj);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    SwapBuffers(*aphDc);
}

void tri::initSimple3Buffers(uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO)
{
    float vertices[] = {
        // Tri 1
        -0.5f, -0.5f, 0.0f,
        0.0f, -0.5f, 0.0f,
        -0.25f, 0.0f, 0.0f,
        // Tri 2
        0.5f, 0.5f, 0.0f,
        0.25f,  0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    uint32_t indices[] =
    {
        0,1,2,3,4,5
        //0, 1, 3, 5, 2, 4
        //1, 2, 3
    };

    _gl::glGenVertexArrays(1, &aVAO);
    _gl::glGenBuffers(1, &aVBO);
    _gl::glGenBuffers(1, &aEBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    _gl::glBindVertexArray(aVAO);

    _gl::glBindBuffer(_gl::GL_ARRAY_BUFFER, aVBO);
    _gl::glBufferData(_gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, _gl::GL_STATIC_DRAW);

    _gl::glBindBuffer(_gl::GL_ELEMENT_ARRAY_BUFFER, aEBO);
    _gl::glBufferData(_gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, _gl::GL_STATIC_DRAW);

    // position attribute
    _gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    _gl::glEnableVertexAttribArray(0);
}

void tri::renderSimple4(HDC* aphDc, const _gl::ShaderData aShaderData)
{
    glClear(GL_COLOR_BUFFER_BIT);
    _gl::glUseProgram(aShaderData.m_cShaderProg);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    SwapBuffers(*aphDc);
}

void tri::initSimple4Buffers(uint32_t& aVBO, uint32_t& aVAO)
{
    float vertices[] =
    {
        // positions        // colors
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
        0.0f, 0.5f, 0.0f,   0.0f, 0.0f, 1.0f   // top
    };

    _gl::glGenVertexArrays(1, &aVAO);
    _gl::glGenBuffers(1, &aVBO);
    _gl::glBindVertexArray(aVAO);

    _gl::glBindBuffer(_gl::GL_ARRAY_BUFFER, aVBO);
    _gl::glBufferData(_gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, _gl::GL_STATIC_DRAW);

    // position attribute
    _gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    _gl::glEnableVertexAttribArray(0);

    // color attribute
    _gl::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    _gl::glEnableVertexAttribArray(1);
}