#pragma once

#include <windows.h>
#include <gl/GL.h>
#include <stdint.h>
#include <cstddef>

namespace _gl
{
	inline constexpr int32_t GL_ARRAY_BUFFER = 0x8892; // decimal: 
	inline constexpr int32_t GL_STATIC_DRAW = 0x88E4; // decimal: 35044

	using GLsizeiptr = std::ptrdiff_t;
	using GLchar = char;

	bool load();
	HGLRC createContextAttribs(HDC aDvcCtxt, HGLRC aRendCtxt, const int32_t* aAttribsLst);
	void glGenBuffers(GLsizei aSize, GLuint* apBuffers);
	void glBindBuffer(GLenum aTarget, GLuint aBuffer);
	void glBufferData(GLenum aTarget, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage);
	GLuint glCreateShader(GLenum aShaderType);
	void glShaderSource(GLuint aShader, GLsizeiptr aCount, const GLchar** apString, const GLint* apLength);
	void glCompileShader(GLuint aShader);
}
