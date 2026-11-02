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

	using PFNGLGETSHADERIVPROC = void (APIENTRY*)(GLuint, GLenum, GLint*);
	PFNGLGETSHADERIVPROC pGlGetShaderivFunc = nullptr;
	constexpr const char* cpGlGetShaderivFuncName = "glGetShaderiv";

	using PFNGLGETSHADERINFOLOGPROC = void (APIENTRY*)(GLuint, GLsizei, GLsizei*, _gl::GLchar*);
	PFNGLGETSHADERINFOLOGPROC pGlGetShaderInfoLogFunc = nullptr;
	constexpr const char* cpGlGetShaderInfoLogFuncName = "glGetShaderInfoLog";

	using PFNGLCREATEPROGRAMPROC = GLuint (APIENTRY*)();
	PFNGLCREATEPROGRAMPROC pGlCreateProgramFunc = nullptr;
	constexpr const char* cpGlCreateProgramFuncName = "glCreateProgram";

	using PFNGLLINKPROGRAMPROC = void (APIENTRY*)(GLuint);
	PFNGLLINKPROGRAMPROC pGlLinkProgramFunc = nullptr;
	constexpr const char* cpGlLinkProgramFuncName = "glLinkProgram";
	
	using PFNGLUSEPROGRAMPROC = void (APIENTRY*)(GLuint);
	PFNGLUSEPROGRAMPROC pGlUseProgramFunc = nullptr;
	constexpr const char* cpGlUseProgramFuncName = "glUseProgram";

	using PFNGLDELETESHADERPROC = void (APIENTRY*)(GLuint);
	PFNGLDELETESHADERPROC pGlDeleteShaderFunc = nullptr;
	constexpr const char* cpGlDeleteShaderFuncName = "glDeleteShader";

	using PFNGLVERTEXATTRIBPOINTERPROC = void (APIENTRY*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*);
	PFNGLVERTEXATTRIBPOINTERPROC pGlVertexAttribPointerFunc = nullptr;
	constexpr const char* cpGlVertexAttribPointerFuncName = "glVertexAttribPointer";

	using PFNGLVERTEXATTRIBIPOINTERPROC = void (APIENTRY*)(GLuint, GLint, GLenum, GLsizei, const GLvoid*);
	PFNGLVERTEXATTRIBIPOINTERPROC pGlVertexAttribIPointerFunc = nullptr;
	constexpr const char* cpGlVertexAttribIPointerFuncName = "glVertexAttribIPointer";

	using PFNGLATTACHSHADERPROC = void (APIENTRY*)(GLuint, GLuint);
	PFNGLATTACHSHADERPROC pGlAttachShaderFunc = nullptr;
	constexpr const char* cpGlAttachShaderFuncName = "glAttachShader";

	using PFNGLGETPROGRAMIVPROC = void (APIENTRY*)(GLuint, GLenum, GLint*);
	PFNGLGETPROGRAMIVPROC pGlGetProgramivFunc = nullptr;
	constexpr const char* cpGlGetProgramivFuncName = "glGetProgramiv";

	using PFNGLGETPROGRAMINFOLOGFUNC = void (APIENTRY*)(GLuint, GLsizei, GLsizei*, _gl::GLchar*);
	PFNGLGETPROGRAMINFOLOGFUNC pGlGetProgramInfoLogFunc = nullptr;
	constexpr const char* cpGlGetProgramInfoLogFuncName = "glGetProgramInfoLog";

	// glVertexAttribPointer
}

bool _gl::load()
{
	pWglCrtCtxtFunc = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress(cpWglCrtCtxtFuncName));
	pGlGenBuffersFunc = reinterpret_cast<PFNGLGENBUFFERSPROC>(wglGetProcAddress(cpGlGenBuffersFuncName));
	pGlBindBufferFunc = reinterpret_cast<PFNGLBINDBUFFERPROC>(wglGetProcAddress(cpGlBindBufferFuncName));
	pGlBufferDataFunc = reinterpret_cast<PFNGLBUFFERDATAPROC>(wglGetProcAddress(cpGlBufferDataFuncName));
	pGlCreateShaderFunc = reinterpret_cast<PFNGLCREATESHADERPROC>(wglGetProcAddress(cpGlCreateShaderFuncName));
	pGlShaderSourceFunc = reinterpret_cast<PFNGLSHADERSOURCEPROC>(wglGetProcAddress(cpGlShaderSourceFuncName));
	pGlCompileShaderFunc = reinterpret_cast<PFNGLCOMPILESHADERPROC>(wglGetProcAddress(cpGlCompileShaderFuncName));
	pGlGetShaderivFunc = reinterpret_cast<PFNGLGETSHADERIVPROC>(wglGetProcAddress(cpGlGetShaderivFuncName));
	pGlGetShaderInfoLogFunc = reinterpret_cast<PFNGLGETSHADERINFOLOGPROC>(wglGetProcAddress(cpGlGetShaderInfoLogFuncName));
	pGlCreateProgramFunc = reinterpret_cast<PFNGLCREATEPROGRAMPROC>(wglGetProcAddress(cpGlCreateProgramFuncName));
	pGlLinkProgramFunc = reinterpret_cast<PFNGLLINKPROGRAMPROC>(wglGetProcAddress(cpGlLinkProgramFuncName));
	pGlUseProgramFunc = reinterpret_cast<PFNGLUSEPROGRAMPROC>(wglGetProcAddress(cpGlUseProgramFuncName));
	pGlDeleteShaderFunc = reinterpret_cast<PFNGLDELETESHADERPROC>(wglGetProcAddress(cpGlDeleteShaderFuncName));
	pGlVertexAttribPointerFunc = reinterpret_cast<PFNGLVERTEXATTRIBPOINTERPROC>(wglGetProcAddress(cpGlVertexAttribPointerFuncName));
	pGlVertexAttribIPointerFunc = reinterpret_cast<PFNGLVERTEXATTRIBIPOINTERPROC>(wglGetProcAddress(cpGlVertexAttribIPointerFuncName));
	pGlAttachShaderFunc = reinterpret_cast<PFNGLATTACHSHADERPROC>(wglGetProcAddress(cpGlAttachShaderFuncName));
	pGlGetProgramivFunc = reinterpret_cast<PFNGLGETPROGRAMIVPROC>(wglGetProcAddress(cpGlGetProgramivFuncName));

	return pWglCrtCtxtFunc && pGlGenBuffersFunc && pGlBindBufferFunc && pGlBufferDataFunc && pGlCreateShaderFunc && pGlShaderSourceFunc && pGlCompileShaderFunc &&
		pGlGetShaderivFunc && pGlGetShaderInfoLogFunc && pGlCreateProgramFunc && pGlLinkProgramFunc && pGlUseProgramFunc && pGlDeleteShaderFunc &&
		pGlVertexAttribPointerFunc && pGlVertexAttribIPointerFunc && pGlAttachShaderFunc && pGlGetProgramivFunc;
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
		return 0;
	}
	return pGlCreateShaderFunc(aShaderType);
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

void _gl::glGetShaderiv(GLuint aShader, GLenum aPname, GLint* apParams)
{
	if (!pGlGetShaderivFunc)
	{
		assert(false);
		return;
	}
	pGlGetShaderivFunc(aShader, aPname, apParams);
}

void _gl::glGetShaderInfoLog(GLuint aShader, GLsizei aMaxLength, GLsizei* apLength, GLchar* apInfoLog)
{
	if (!pGlGetShaderInfoLogFunc)
	{
		assert(false);
		return;
	}
	pGlGetShaderInfoLogFunc(aShader, aMaxLength, apLength, apInfoLog);
}

GLuint _gl::glCreateProgram()
{
	if (!pGlCreateProgramFunc)
	{
		assert(false);
		return 0;
	}
	return pGlCreateProgramFunc();
}

void _gl::glLinkProgram(GLuint aProgram)
{
	if (!pGlLinkProgramFunc)
	{
		assert(false);
		return;
	}
	pGlLinkProgramFunc(aProgram);
}

void _gl::glUseProgram(GLuint aProgram)
{
	if (!pGlUseProgramFunc)
	{
		assert(false);
		return;
	}
	pGlUseProgramFunc(aProgram);
}

void _gl::glDeleteShader(GLuint aShader)
{
	if (!pGlDeleteShaderFunc)
	{
		assert(false);
		return;
	}
	pGlDeleteShaderFunc(aShader);
}

void _gl::glVertexAttribPointer(GLuint aIndex, GLint aSize, GLenum aType, GLboolean aNormalized, GLsizei aStride, const GLvoid* apPointer)
{
	if (!pGlVertexAttribPointerFunc)
	{
		assert(false);
		return;
	}
	pGlVertexAttribPointerFunc(aIndex, aSize, aType, aNormalized, aStride, apPointer);
}

void _gl::glVertexAttribIPointer(GLuint aIndex, GLint aSize, GLenum aType, GLsizei aStride, const GLvoid* apPointer)
{
	if (!pGlVertexAttribIPointerFunc)
	{
		assert(false);
		return;
	}
	pGlVertexAttribIPointerFunc(aIndex, aSize, aType, aStride, apPointer);
}

void _gl::glAttachShader(GLuint aProgram, GLuint aShader)
{
	if (!pGlAttachShaderFunc)
	{
		assert(false);
		return;
	}
	pGlAttachShaderFunc(aProgram, aShader);
}

void _gl::glGetProgramiv(GLuint aProgram, GLenum aPname, GLint* apParams)
{
	if (!pGlGetProgramivFunc)
	{
		assert(false);
		return;
	}
	pGlGetProgramivFunc(aProgram, aPname, apParams);
}

void _gl::glGetProgramInfoLog(GLuint aProgram, GLsizei aMaxLength, GLsizei* apLength, _gl::GLchar* apInfoLog)
{
	if (!pGlGetProgramInfoLogFunc)
	{
		assert(false);
		return;
	}
	pGlGetProgramInfoLogFunc(aProgram, aMaxLength, apLength, apInfoLog);
}
