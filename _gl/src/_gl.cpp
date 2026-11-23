#include "pch.h"
#include "_gl.h"
#include <utils.h>
#include <logger.h>
#include <cassert>
#include <stdexcept>
#include <format>

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

	using PFNGLGETPROGRAMINFOLOGPROC = void (APIENTRY*)(GLuint, GLsizei, GLsizei*, _gl::GLchar*);
	PFNGLGETPROGRAMINFOLOGPROC pGlGetProgramInfoLogFunc = nullptr;
	constexpr const char* cpGlGetProgramInfoLogFuncName = "glGetProgramInfoLog";

	using PFNGLENABLEVERTEXATTRIBARRAYPROC = void (APIENTRY*)(GLuint);
	PFNGLENABLEVERTEXATTRIBARRAYPROC pGlEnableVertexAttribArrayFunc = nullptr;
	constexpr const char* cpGlEnableVertexAttribArrayFuncName = "glEnableVertexAttribArray";

	using PFNGLDISABLEVERTEXATTRIBARRAYPROC = void (APIENTRY*)(GLuint);
	PFNGLDISABLEVERTEXATTRIBARRAYPROC pGlDisableVertexAttribArrayFunc = nullptr;
	constexpr const char* cpGlDisableVertexAttribArrayFuncName = "glDisableVertexAttribArray";

	using PFNGLDELETEVERTEXARRAYSPROC = void (APIENTRY*)(GLsizei, const GLuint*);
	PFNGLDELETEVERTEXARRAYSPROC pGlDeleteVertexArraysFunc = nullptr;
	constexpr const char* cpGlDeleteVertexArraysFuncName = "glDeleteVertexArrays";
	
	using PFNGLDELETEBUFFERSPROC = void (APIENTRY*)(GLsizei, const GLuint*);
	PFNGLDELETEBUFFERSPROC pGlDeleteBuffersFunc = nullptr;
	constexpr const char* cpGlDeleteBuffersFuncName = "glDeleteBuffers";

	using PFNGLDELETEPROGRAMPROC = void (APIENTRY*)(GLuint);
	PFNGLDELETEPROGRAMPROC pGlDeleteProgramFunc = nullptr;
	constexpr const char* cpGlDeleteProgramFuncName = "glDeleteProgram";

	using PFNGLGENVERTEXARRAYSPROC = void (APIENTRY*)(GLsizei, GLuint*);
	PFNGLGENVERTEXARRAYSPROC pGlGenVertexArraysFunc = nullptr;
	constexpr const char* cpGlGenVertexArraysFuncName = "glGenVertexArrays";

	using PFNGLBINDVERTEXARRAYPROC = void (APIENTRY*)(GLuint);
	PFNGLBINDVERTEXARRAYPROC pGlBindVertexArrayFunc = nullptr;
	constexpr const char* cpGlBindVertexArrayFuncName = "glBindVertexArray";

	using PFNGLGETUNIFORMLOCATIONPROC = GLint (APIENTRY*)(GLuint, const _gl::GLchar*);
	PFNGLGETUNIFORMLOCATIONPROC pGlGetUniformLocationFunc = nullptr;
	constexpr const char* cpGlGetUniformLocationFuncName = "glGetUniformLocation";

	using PFNGLUNIFORM4FPROC = void (APIENTRY*)(GLint, GLfloat, GLfloat, GLfloat, GLfloat);
	PFNGLUNIFORM4FPROC pGlUniform4fFunc = nullptr;
	constexpr const char* cpGlUniform4fFuncName = "glUniform4f";

	using PFNGLTEXPARAMETERFPROC = void (APIENTRY*)(GLenum, GLenum, GLfloat);
	PFNGLTEXPARAMETERFPROC pGlTexParameterfFunc = nullptr;
	constexpr const char* cpGlTexParameterfFuncName = "glTexParameterf";

	using PFNGLTEXPARAMETERIPROC = void (APIENTRY*)(GLenum, GLenum, GLint);
	PFNGLTEXPARAMETERIPROC pGlTexParameteriFunc = nullptr;
	constexpr const char* cpGlTexParameteriFuncName = "glTexParameteri";

	using PFNGLTEXPARAMETERFVPROC = void (APIENTRY*)(GLenum, GLenum, const GLfloat*);
	PFNGLTEXPARAMETERFVPROC pGlTexParameterfvFunc = nullptr;
	constexpr const char* cpGlTexParameterfvFuncName = "glTexParameterfv";

	using PFNGLTEXPARAMETERIVPROC = void (APIENTRY*)(GLenum, GLenum, const GLint*);
	PFNGLTEXPARAMETERIVPROC pGlTexParameterivFunc = nullptr;
	constexpr const char* cpGlTexParameterivFuncName = "glTexParameteriv";

	using PFNGLGENERATEMIPMAPPROC = void (APIENTRY*)(GLenum);
	PFNGLGENERATEMIPMAPPROC pGlGenerateMipmapFunc = nullptr;
	constexpr const char* cpGlGenerateMipmapFuncName = "glGenerateMipmap";
}

//========== Startup / Shutdown ==========

bool _gl::initOpenGL(HWND ahWnd, HDC* aphDc, HGLRC* aphGlrc, int32_t aVpWidth, int32_t aVpHeight)
{
	utils::log("shuba");

	PIXELFORMATDESCRIPTOR pixFmtDesc{};
	pixFmtDesc.nSize = sizeof(pixFmtDesc);
	pixFmtDesc.nVersion = 1;
	pixFmtDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixFmtDesc.iPixelType = PFD_TYPE_RGBA;
	pixFmtDesc.cColorBits = 32;
	pixFmtDesc.cDepthBits = 24;
	pixFmtDesc.cStencilBits = 8;
	pixFmtDesc.iLayerType = PFD_MAIN_PLANE;

	*aphDc = GetDC(ahWnd);
	if (!*aphDc)
	{
		return false;
	}

	const int32_t pixFmt = ChoosePixelFormat(*aphDc, &pixFmtDesc);
	if (pixFmt == 0 || !SetPixelFormat(*aphDc, pixFmt, &pixFmtDesc))
	{
		return false;
	}

	HGLRC tempGlrc = wglCreateContext(*aphDc);
	if (!tempGlrc || !wglMakeCurrent(*aphDc, tempGlrc))
	{
		if (tempGlrc)
		{
			wglDeleteContext(tempGlrc);
		}

		return false;
	}

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

	if (!_gl::loadWglCrtCtxtFunction())
	{
		//utils::errMsg(g_wHndl, "Failed to load WGL context creation function!", "WGL Load Failed!");
		return false;
	}

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

	if (!_gl::loadOpenGlFunctions())
	{
		//utils::errMsg(g_wHndl, "Failed to load OpenGL functions!", "OpenGL Load Failed!");
		return false;
	}

	// wireframe rendering mode:
	// arg 1: apply to front and back of all triangles
	// arg 2: draw them as lines
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) 
	// 
	// revert to normal:
	// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)

	glViewport(0, 0, aVpWidth, aVpHeight);
	return true;
}

void _gl::shutdownOpenGL(HWND ahWnd, HDC& ahDc, HGLRC& ahGlrc)
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

//========== Function Loaders / OpenGL Context Creation ==========

bool _gl::loadWglCrtCtxtFunction()
{
	pWglCrtCtxtFunc = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress(cpWglCrtCtxtFuncName));

	return pWglCrtCtxtFunc;
}

bool _gl::loadOpenGlFunctions()
{
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
	pGlEnableVertexAttribArrayFunc = reinterpret_cast<PFNGLENABLEVERTEXATTRIBARRAYPROC>(wglGetProcAddress(cpGlEnableVertexAttribArrayFuncName));
	pGlDisableVertexAttribArrayFunc = reinterpret_cast<PFNGLDISABLEVERTEXATTRIBARRAYPROC>(wglGetProcAddress(cpGlDisableVertexAttribArrayFuncName));
	pGlDeleteVertexArraysFunc = reinterpret_cast<PFNGLDELETEVERTEXARRAYSPROC>(wglGetProcAddress(cpGlDeleteVertexArraysFuncName));
	pGlDeleteBuffersFunc = reinterpret_cast<PFNGLDELETEBUFFERSPROC>(wglGetProcAddress(cpGlDeleteBuffersFuncName));
	pGlDeleteProgramFunc = reinterpret_cast<PFNGLDELETEPROGRAMPROC>(wglGetProcAddress(cpGlDeleteProgramFuncName));
	pGlGenVertexArraysFunc = reinterpret_cast<PFNGLGENVERTEXARRAYSPROC>(wglGetProcAddress(cpGlGenVertexArraysFuncName));
	pGlBindVertexArrayFunc = reinterpret_cast<PFNGLBINDVERTEXARRAYPROC>(wglGetProcAddress(cpGlBindVertexArrayFuncName));
	pGlGetUniformLocationFunc = reinterpret_cast<PFNGLGETUNIFORMLOCATIONPROC>(wglGetProcAddress(cpGlGetUniformLocationFuncName));
	pGlUniform4fFunc = reinterpret_cast<PFNGLUNIFORM4FPROC>(wglGetProcAddress(cpGlUniform4fFuncName));

	return pGlGenBuffersFunc && pGlBindBufferFunc && pGlBufferDataFunc && pGlCreateShaderFunc && pGlShaderSourceFunc && pGlCompileShaderFunc &&
		pGlGetShaderivFunc && pGlGetShaderInfoLogFunc && pGlCreateProgramFunc && pGlLinkProgramFunc && pGlUseProgramFunc && pGlDeleteShaderFunc &&
		pGlVertexAttribPointerFunc && pGlVertexAttribIPointerFunc && pGlAttachShaderFunc && pGlGetProgramivFunc && pGlEnableVertexAttribArrayFunc && 
		pGlDisableVertexAttribArrayFunc && pGlGenVertexArraysFunc && pGlBindVertexArrayFunc && pGlGetUniformLocationFunc && pGlUniform4fFunc &&
		pGlTexParameterfFunc && pGlTexParameteriFunc && pGlTexParameterfvFunc && pGlTexParameterivFunc;
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

//========== Helpers ==========

//bool _gl::loadShaderSrc(const char* apVtxShdrSrcPath, string& aVertShaderSrc, const char* apFrgShdrSrcPath, string& aFragShaderSrc)
bool _gl::loadShaderSrc(const char* apShdrSrcPath, std::string& aShdrSrc)
{
	try
	{
		utils::loadTxtFile(apShdrSrcPath, aShdrSrc);
	}
	catch (std::runtime_error ex)
	{
		//std::string errorMsg = std::format("Failed to load source code for vertex shader. Error message:\n\n\"{}\"\n\nExiting...", *ex.what());
		//utils::errMsg(g_wHndl, errorMsg.c_str(), "Shader Src Load Failed!");
		aShdrSrc = "";
	}

	return aShdrSrc != "";
}

//========== OpenGL 3.3 Functions ==========

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

void _gl::glEnableVertexAttribArray(GLuint aIndex)
{
	if (!pGlEnableVertexAttribArrayFunc)
	{
		assert(false);
		return;
	}
	pGlEnableVertexAttribArrayFunc(aIndex);
}

void _gl::glDisableVertexAttribArray(GLuint aIndex)
{
	if (!pGlDisableVertexAttribArrayFunc)
	{
		assert(false);
		return;
	}
	pGlDisableVertexAttribArrayFunc(aIndex);
}

void _gl::glDeleteVertexArrays(GLsizei aN, const GLuint* apArrays)
{
	if (!pGlDeleteVertexArraysFunc)
	{
		assert(false);
		return;
	}
	pGlDeleteVertexArraysFunc(aN, apArrays);
}

void _gl::glDeleteBuffers(GLsizei aN, const GLuint* apBuffers)
{
	if (!pGlDeleteBuffersFunc)
	{
		assert(false);
		return;
	}
	pGlDeleteBuffersFunc(aN, apBuffers);
}

void _gl::glDeleteProgram(GLuint aProgram)
{
	if (!pGlDeleteProgramFunc)
	{
		assert(false);
		return;
	}
	pGlDeleteProgramFunc(aProgram);
}

void _gl::glGenVertexArrays(GLsizei aN, GLuint* apArrays)
{
	if (!pGlGenVertexArraysFunc)
	{
		assert(false);
		return;
	}
	pGlGenVertexArraysFunc(aN, apArrays);
}

void _gl::glBindVertexArray(GLuint aArray)
{
	if (!pGlBindVertexArrayFunc)
	{
		assert(false);
		return;
	}
	pGlBindVertexArrayFunc(aArray);
}

GLint _gl::glGetUniformLocation(GLuint aProgram, const GLchar* apName)
{
	if (!pGlGetUniformLocationFunc)
	{
		assert(false);
		return -1;
	}
	return pGlGetUniformLocationFunc(aProgram, apName);
}

void _gl::glUniform4f(GLint aLocation, GLfloat aVal0, GLfloat aVal1, GLfloat aVal2, GLfloat aVal3)
{
	if (!pGlUniform4fFunc)
	{
		assert(false);
		return;
	}
	pGlUniform4fFunc(aLocation, aVal0, aVal1, aVal2, aVal3);
}

void _gl::glGenerateMipmap(GLenum aTarget)
{
	if (!pGlGenerateMipmapFunc)
	{
		assert(false);
		return;
	}
	pGlGenerateMipmapFunc(aTarget);
}
