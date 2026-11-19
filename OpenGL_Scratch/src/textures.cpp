#include "textures.h"
#include "_gl.h"

void tx::initWoodRectBuffers()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _gl::GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _gl::GL_MIRRORED_REPEAT);

	float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glTexParameteri(GL_PROXY_TEXTURE_2D, );
	glTexParameteri(GL_PROXY_TEXTURE_2D, );

	float textureCoords[] =
	{
		0.0f, 0.0f, // lower left corner
		1.0f, 0.0f, // lower right corner
		0.5f, 1.0f, // top center corner
	};
}

void tx::renderWoodRect()
{

}