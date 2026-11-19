#pragma once

#include "framework.h"
#include <gl/GL.h>
#include <stdint.h>
#include <cstddef>
#include <string>

#if defined(GL_EXPORTS)
#define GL_API __declspec(dllexport)
#else
#define GL_API __declspec(dllimport)
#endif

namespace _gl
{
	inline constexpr int32_t GL_ARRAY_BUFFER = 0x8892;
	inline constexpr int32_t GL_STATIC_DRAW = 0x88E4;
	inline constexpr int32_t GL_VERTEX_SHADER = 0x8B31;
	inline constexpr int32_t GL_FRAGMENT_SHADER = 0x8B30;
	inline constexpr int32_t GL_COMPILE_STATUS = 0x8B81;
	inline constexpr int32_t GL_LINK_STATUS = 0x8B82;
	inline constexpr int32_t GL_ELEMENT_ARRAY_BUFFER = 0x8893;
	inline constexpr int32_t GL_MAX_VERTEX_ATTRIBS = 0x8869;
	inline constexpr int32_t GL_MIRRORED_REPEAT = 0x8370;

	using GLsizeiptr = std::ptrdiff_t;
	using GLchar = char;

	//========== Startup / Shutdown ==========
	GL_API bool initOpenGL(HWND ahWnd, HDC* aphDc, HGLRC* aphGlrc, int32_t aVpWidth, int32_t aVpHeight);
	GL_API void shutdownOpenGL(HWND ahWnd, HDC& ahDc, HGLRC& ahGlrc);

	//========== Function Loaders / OpenGL Context Creation ==========
	GL_API bool loadWglCrtCtxtFunction();
	GL_API bool loadOpenGlFunctions();
	GL_API HGLRC createContextAttribs(HDC aDvcCtxt, HGLRC aRendCtxt, const int32_t* aAttribsLst);

	//========== Helpers ==========
	GL_API bool loadShaderSrc(const char* apShdrSrcPath, std::string& aShdrSrc);

	//========== OpenGL 3.3 Functions ==========
	GL_API void glGenBuffers(GLsizei aSize, GLuint* apBuffers);
	GL_API void glBindBuffer(GLenum aTarget, GLuint aBuffer);
	GL_API void glBufferData(GLenum aTarget, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage);
	GL_API GLuint glCreateShader(GLenum aShaderType);
	GL_API void glShaderSource(GLuint aShader, GLsizeiptr aCount, const GLchar** apString, const GLint* apLength);
	GL_API void glCompileShader(GLuint aShader);
	GL_API void glGetShaderiv(GLuint aShader, GLenum aPname, GLint* apParams);
	GL_API void glGetShaderInfoLog(GLuint aShader, GLsizei aMaxLength, GLsizei* apLength, GLchar* apInfoLog);
	GL_API GLuint glCreateProgram();
	GL_API void glLinkProgram(GLuint aProgram);
	GL_API void glUseProgram(GLuint aProgram);
	GL_API void glDeleteShader(GLuint aShader);
	GL_API void glVertexAttribPointer(GLuint aIndex, GLint aSize, GLenum aType, GLboolean aNormalized, GLsizei aStride, const GLvoid* apPointer);
	GL_API void glVertexAttribIPointer(GLuint aIndex, GLint aSize, GLenum aType, GLsizei aStride, const GLvoid* apPointer);
	GL_API void glAttachShader(GLuint aProgram, GLuint aShader);
	GL_API void glGetProgramiv(GLuint aProgram, GLenum aPname, GLint* apParams);
	GL_API void glGetProgramInfoLog(GLuint aProgram, GLsizei aMaxLength, GLsizei* apLength, _gl::GLchar* apInfoLog);
	GL_API void glEnableVertexAttribArray(GLuint aIndex);
	GL_API void glDisableVertexAttribArray(GLuint aIndex);
	GL_API void glDeleteVertexArrays(GLsizei aN, const GLuint* apArrays);
	GL_API void glDeleteBuffers(GLsizei aN, const GLuint* apBuffers);
	GL_API void glDeleteProgram(GLuint aProgram);
	GL_API void glGenVertexArrays(GLsizei aN, GLuint* apArrays);
	GL_API void glBindVertexArray(GLuint aArray);
	GL_API GLint glGetUniformLocation(GLuint aProgram, const GLchar* apName);
	GL_API void glUniform4f(GLint aLocation, GLfloat aVal0, GLfloat aVal1, GLfloat aVal2, GLfloat aVal3);
	//GL_API void glTexParameterf(GLenum aTarget, GLenum aName, GLfloat aParam);
	//GL_API void glTexParameteri(GLenum aTarget, GLenum aName, GLint aParam);
	//GL_API void glTexParameterfv(GLenum aTarget, GLenum aName, const GLfloat* apParams);
	//GL_API void glTexParameteriv(GLenum aTarget, GLenum aName, const GLint* apParams);

	struct ShaderData
	{
		ShaderData() : 
			m_cShaderProg(0), 
			m_cVtxBffrObj(0), 
			m_cVtxArryObj(0), 
			m_vElemBffrObj(0) 
		{}

		ShaderData(uint32_t aShaderProg, uint32_t aVBO, uint32_t aVAO, uint32_t aEBO) :
			m_cShaderProg(aShaderProg),
			m_cVtxBffrObj(aVBO),
			m_cVtxArryObj(aVAO),
			m_vElemBffrObj(aEBO)
		{}
		
		const uint32_t m_cShaderProg;
		const uint32_t m_cVtxBffrObj;
		const uint32_t m_cVtxArryObj;
		const uint32_t m_vElemBffrObj;
	};
}
