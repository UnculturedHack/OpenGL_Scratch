#include "../include/_gl.h"
#include <cassert>

namespace
{
	using PFNWGLCREATECONTEXTATTRIBSARBPROC = HGLRC (WINAPI*)(HDC, HGLRC, const int32_t*);
	PFNWGLCREATECONTEXTATTRIBSARBPROC pWglCrtCtxtFunc = nullptr;
	constexpr const char* cpWglCrtCtxtFuncName = "wglCreateContextAttribsARB";

	using PFNGLGENBUFFERSPROC = void (APIENTRY*)(GLsizei, GLuint*);
	PFNGLGENBUFFERSPROC pGlGenBuffersFunc = nullptr;
	constexpr const char* cpGlGenBuffersFuncName = "glGenBuffers";

	using PFNGLBINDBUFFERPROC = void (APIENTRY*)(GLenum, GLuint);
	PFNGLBINDBUFFERPROC pGlBindBufferFunc = nullptr;
	constexpr const char* cpGlBindBufferFuncName = "glBindBuffer";

	using PFNGLBUFFERDATAPROC = void (APIENTRY*)(GLenum, _gl::GLsizeiptr, const GLvoid*, GLenum);
	PFNGLBUFFERDATAPROC pGlBufferDataFunc = nullptr;
	constexpr const char* cpGlBufferDataFuncName = "glBufferData";

	using PFNGLCREATESHADERPROC = GLuint (APIENTRY*)(GLenum);
	PFNGLCREATESHADERPROC pGlCreateShaderFunc = nullptr;
	constexpr const char* cpGlCreateShaderFuncName = "glCreateShader";

	using PFNGLSHADERSOURCEPROC = void (APIENTRY*)(GLuint aShader, _gl::GLsizeiptr aCount, const _gl::GLchar** apString, const GLint* apLength);
	PFNGLSHADERSOURCEPROC pGlShaderSourceFunc = nullptr;
	constexpr const char* cpGlShaderSourceFuncName = "glShaderSource";

	using PFNGLCOMPILESHADERPROC = void (APIENTRY*)(GLuint aShader);
	PFNGLCOMPILESHADERPROC pGlCompileShaderFunc = nullptr;
	constexpr const char* cpGlCompileShaderFuncName = "glCompileShader";
}

bool _gl::load()
{
	pWglCrtCtxtFunc = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress(cpWglCrtCtxtFuncName));
	pGlGenBuffersFunc = reinterpret_cast<PFNGLGENBUFFERSPROC>(wglGetProcAddress(cpGlGenBuffersFuncName));
	pGlBindBufferFunc = reinterpret_cast<PFNGLBINDBUFFERPROC>(wglGetProcAddress(cpGlBindBufferFuncName));
	pGlBufferDataFunc = reinterpret_cast<PFNGLBUFFERDATAPROC>(wglGetProcAddress(cpGlBufferDataFuncName));
	return pWglCrtCtxtFunc && pGlGenBuffersFunc && pGlBindBufferFunc && pGlBufferDataFunc;
}

HGLRC _gl::createContextAttribs(HDC aDvcCtxt, HGLRC aRendCtxt, const int32_t* aAttribsLst)
{
	if (!pWglCrtCtxtFunc)
	{
		assert(false);
		return nullptr;
	}
	return pWglCrtCtxtFunc(aDvcCtxt, aRendCtxt, aAttribsLst);
}

void _gl::glGenBuffers(GLsizei aSize, GLuint* apBuffer)
{
	if (!pGlGenBuffersFunc)
	{
		assert(false);
		return;
	}
	pGlGenBuffersFunc(aSize, apBuffer);
}

void _gl::glBindBuffer(GLenum aTarget, GLuint aBuffer)
{
	if (!pGlBindBufferFunc)
	{
		assert(false);
		return;
	}
	pGlBindBufferFunc(aTarget, aBuffer);
}

void _gl::glBufferData(GLenum aTarget, GLsizeiptr aSize, const GLvoid* aData, GLenum aUsage)
{
	if (!pGlBufferDataFunc)
	{
		assert(false);
		return;
	}
	pGlBufferDataFunc(aTarget, aSize, aData, aUsage);
}

GLuint _gl::glCreateShader(GLenum aShaderType)
{
	if (!pGlCreateShaderFunc)
	{
		assert(false);
		return;
	}
	pGlCreateShaderFunc(aShaderType);
}

void _gl::glShaderSource(GLuint aShader, GLsizeiptr aCount, const GLchar** apString, const GLint* apLength)
{
	if (!pGlShaderSourceFunc)
	{
		assert(false);
		return;
	}
	pGlShaderSourceFunc(aShader, aCount, apString, apLength);
}

void _gl::glCompileShader(GLuint aShader)
{
	if (!pGlCompileShaderFunc)
	{
		assert(false);
		return;
	}
	pGlCompileShaderFunc(aShader);
}