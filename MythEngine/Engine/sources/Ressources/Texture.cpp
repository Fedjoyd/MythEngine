#include "Ressources/Texture.h"
#include "Core/Debug/Log.h"
#include <stb_image.h>

void Ressources::Texture::LoadRessource()
{
	int width, height, nrChannels;
	m_gpu = new GPUTexture;

	unsigned char* data = stbi_load(m_path.c_str(), &width, &height, &nrChannels, 0);
	const char* txt = m_path.c_str();

	glGenTextures(1, &m_gpu->data);

	if (data)
	{
		GLenum format = 0;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_gpu->data);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	else
	{
		MYTH_ERROR("Image loading failed on '%s'\n", m_path.c_str());
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_loaded = true;
	return;
}

void Ressources::Texture::UnloadRessource()
{
	glDeleteTextures(1, &m_gpu->data);
	delete(m_gpu);
	m_loaded = false;
}
