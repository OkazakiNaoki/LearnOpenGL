#ifndef TEXLOADER_H
#define TEXLOADER_H

#include <glad/glad.h>
#include <string>
#include "header/stb_image.h"

class TexLoader {
public:
	TexLoader() {}
	TexLoader(char const * mapPath, GLint wrap, GLint minFilter, GLint magFilter);
	unsigned int GetMapID();
private:
	unsigned int mapID;
};
#endif