#define STB_IMAGE_IMPLEMENTATION
#include "header/TexLoader.h"
#include <iostream>

TexLoader::TexLoader(char const * mapPath, GLint wrap, GLint minFilter, GLint magFilter) {
	glGenTextures(1, &mapID);
	int texW, texH, nrChannels;
	unsigned char *data = stbi_load(mapPath, &texW, &texH, &nrChannels, 0);
	if (data) {
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, mapID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		glTexImage2D(GL_TEXTURE_2D, 0, format, texW, texH, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	}
	else {
		std::cout << "Fail loading image." << std::endl;
		stbi_image_free(data);
	}
}

unsigned int TexLoader::GetMapID() {
	return mapID;
}