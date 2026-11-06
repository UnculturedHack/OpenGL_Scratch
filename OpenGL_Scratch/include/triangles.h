#pragma once

#include <stdint.h>
#include <Windows.h>

namespace _gl
{
	struct ShaderData;
}

namespace tri
{
	// for simpleVert.glsl and simpleFrag.glsl
	void initSimpleBuffers(uint32_t& aVBO, uint32_t& aVAO);
	void renderSimple(HDC* aphDc, const _gl::ShaderData aShaderData);

	// for simpleVert2.glsl and simpleFrag2.glsl
	void initSimple2Buffers(uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO);
	void renderSimple2(HDC* aphDc, const _gl::ShaderData aShaderData);

	// for simpleVert3.glsl and simpleFrag3.glsl
	void initSimple3Buffers(uint32_t& aVBO, uint32_t& aVAO, uint32_t& aEBO);
	void renderSimple3(HDC* aphDc, const _gl::ShaderData aShaderData);

	// for simpleVert4.glsl and simpleFrag4.glsl
	void initSimple4Buffers(uint32_t& aVBO, uint32_t& aVAO);
	void renderSimple4(HDC* aphDc, const _gl::ShaderData aShaderData);
}