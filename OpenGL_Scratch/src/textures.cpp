#include "textures.h"
#include "_gl.h"
#include "stb_image.h"
#include "utils.h"

static void loadTexture(const char* apTxtrFilePath)
{
	uint32_t texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int32_t width;
	int32_t height;
	int32_t nrChannels;
	unsigned char* pData = stbi_load(apTxtrFilePath, &width, &height, &nrChannels, 0);
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

void tx::initWoodRect(uint32_t& aVBO, uint32_t& aVAO)
{
	float vertices[] =
	{
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	uint32_t indices[] = 
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	uint32_t VBO;
	uint32_t VAO, EBO;
	_gl::glGenVertexArrays(1, &VAO);
	_gl::glGenBuffers(1, &VBO);
	_gl::glGenBuffers(1, &EBO);

	_gl::glBindVertexArray(VAO);

	_gl::glBindBuffer(_gl::GL_ARRAY_BUFFER, VBO);
	_gl::glBufferData(_gl::GL_ARRAY_BUFFER, sizeof(vertices), vertices, _gl::GL_STATIC_DRAW);

	_gl::glBindBuffer(_gl::GL_ELEMENT_ARRAY_BUFFER, EBO);
	_gl::glBufferData(_gl::GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, _gl::GL_STATIC_DRAW);

	// position attribute
	_gl::glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	_gl::glEnableVertexAttribArray(0);
	// color attribute
	_gl::glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	_gl::glEnableVertexAttribArray(1);
	// texture coord attribute
	_gl::glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	_gl::glEnableVertexAttribArray(2);

	loadTexture("assets/textures/container.jpg");
}

void tx::renderWoodRect(HDC* aphDc, const _gl::ShaderData aShaderData)
{
	glClear(GL_COLOR_BUFFER_BIT);
	_gl::glUseProgram(aShaderData.m_cShaderProg);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	SwapBuffers(*aphDc);
}

