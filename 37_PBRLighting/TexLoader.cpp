#define STB_IMAGE_IMPLEMENTATION
#include "header/TexLoader.h"
#include <iostream>


TexLoader::TexLoader(char const * mapPath, GLint wrap, GLint minFilter, GLint magFilter, bool sRGB) {
	glGenTextures(1, &mapID);
	int texW, texH, nrChannels;
	unsigned char *data = stbi_load(mapPath, &texW, &texH, &nrChannels, 0);
	if (data) {
		GLenum format;
		GLenum pixFormat;
		if (nrChannels == 1) {
			pixFormat = GL_RED;
			format = GL_RED;
		}
		else if (nrChannels == 3) {
			pixFormat = GL_RGB;
			if (!sRGB)
				format = GL_RGB;
			else
				format = GL_SRGB;
		}
		else if (nrChannels == 4) {
			pixFormat = GL_RGBA;
			if (!sRGB)
				format = GL_RGBA;
			else
				format = GL_SRGB_ALPHA;
		}
		glBindTexture(GL_TEXTURE_2D, mapID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texW, texH, 0, pixFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Fail loading image." << std::endl;
		stbi_image_free(data);
	}
}

TexLoader::TexLoader(std::vector<std::string> faces, GLint minFilter, GLint magFilter)
{
	glGenTextures(1, &mapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mapID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


unsigned int TexLoader::GetMapID() {
	return mapID;
}