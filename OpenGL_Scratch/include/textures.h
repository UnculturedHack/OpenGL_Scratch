#pragma once

#include <stdint.h>
#include <Windows.h>

namespace _gl
{
	struct ShaderData;
}

namespace tx
{
	// 
	void initWoodRect(uint32_t& aVBO, uint32_t& aVAO);
	void renderWoodRect(HDC* aphDc, const _gl::ShaderData aShaderData);

	//
}