#include "textures.h"
#include "_gl.h"
#include "stb_image.h"
#include "utils.h"

static void loadTexture()
{
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int32_t width;
	int32_t height;
	int32_t nrChannels;
	unsigned char* pData = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);
	if (pData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
		_gl::glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		utils::printfDbg("Failed to load texture");
	}

	stbi_image_free(pData);
}

void tx::initWoodRectShaders(uint32_t& aVBO, uint32_t& aVAO)
{
	// TODO: init shader buffers and all that

	float vertices[] =
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	loadTexture();
}

void tx::renderWoodRect(HDC* aphDc, const _gl::ShaderData aShaderData)
{

}

