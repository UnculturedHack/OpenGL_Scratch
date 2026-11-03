#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <stdint.h>
#include <cstddef>

namespace _gl
{
	inline constexpr int32_t GL_ARRAY_BUFFER = 0x8892;
	inline constexpr int32_t GL_STATIC_DRAW = 0x88E4;
	inline constexpr int32_t GL_VERTEX_SHADER = 0x8B31;
	inline constexpr int32_t GL_FRAGMENT_SHADER = 0x8B30;
	inline constexpr int32_t GL_COMPILE_STATUS = 0x8B81;
	inline constexpr int32_t GL_LINK_STATUS = 0x8B82;
	inline constexpr int32_t GL_ELEMENT_ARRAY_BUFFER = 0x8893;

	using GLsizeiptr = std::ptrdiff_t;
	using GLchar = char;

	bool loadWglCrtCtxtFunction();
	bool loadOpenGlFunctions();
	HGLRC createContextAttribs(HDC aDvcCtxt, HGLRC aRendCtxt, const int32_t* aAttribsLst);
	void glGenBuffers(GLsizei aSize, GLuint* apBuffers);
	void glBindBuffer(GLenum aTarget, GLuint aBuffer);
	void glBufferData(GLenum aTarget, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage);
	GLuint glCreateShader(GLenum aShaderType);
	void glShaderSource(GLuint aShader, GLsizeiptr aCount, const GLchar** apString, const GLint* apLength);
	void glCompileShader(GLuint aShader);
	void glGetShaderiv(GLuint aShader, GLenum aPname, GLint* apParams);
	void glGetShaderInfoLog(GLuint aShader, GLsizei aMaxLength, GLsizei* apLength, GLchar* apInfoLog);
	GLuint glCreateProgram();
	void glLinkProgram(GLuint aProgram);
	void glUseProgram(GLuint aProgram);
	void glDeleteShader(GLuint aShader);
	void glVertexAttribPointer(GLuint aIndex, GLint aSize, GLenum aType, GLboolean aNormalized, GLsizei aStride, const GLvoid* apPointer);
	void glVertexAttribIPointer(GLuint aIndex, GLint aSize, GLenum aType, GLsizei aStride, const GLvoid* apPointer);
	void glAttachShader(GLuint aProgram, GLuint aShader);
	void glGetProgramiv(GLuint aProgram, GLenum aPname, GLint* apParams);
	void glGetProgramInfoLog(GLuint aProgram, GLsizei aMaxLength, GLsizei* apLength, _gl::GLchar* apInfoLog);
	void glEnableVertexAttribArray(GLuint aIndex);
	void glDisableVertexAttribArray(GLuint aIndex);
	void glDeleteVertexArrays(GLsizei aN, const GLuint* apArrays);
	void glDeleteBuffers(GLsizei aN, const GLuint* apBuffers);
	void glDeleteProgram(GLuint aProgram);
	void glGenVertexArrays(GLsizei aN, GLuint* apArrays);
	void glBindVertexArray(GLuint aArray);

	struct ShaderData
	{
		ShaderData() : 
			m_shaderProg(0), 
			m_vtxBffrObj(0), 
			m_vtxArryObj(0), 
			m_elemBffrObj(0) 
		{}

		ShaderData(uint32_t aShaderProg, uint32_t aVBO, uint32_t aVAO, uint32_t aEBO) :
			m_shaderProg(aShaderProg),
			m_vtxBffrObj(aVBO),
			m_vtxArryObj(aVAO),
			m_elemBffrObj(aEBO)
		{}
		
		const uint32_t m_shaderProg;
		const uint32_t m_vtxBffrObj;
		const uint32_t m_vtxArryObj;
		const uint32_t m_elemBffrObj;
	};
}
