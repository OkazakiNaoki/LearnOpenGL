#ifndef TEXLOADER_H
#define TEXLOADER_H

#include <glad/glad.h>
#include <string>
#include <vector>
#include "header/stb_image.h"

class TexLoader {
public:
	TexLoader() {}
	TexLoader(char const * mapPath, GLint wrap, GLint minFilter, GLint magFilter, bool sRGB);
	TexLoader(std::vector<std::string> faces, GLint minFilter, GLint magFilter);
	unsigned int GetMapID();
private:
	unsigned int mapID;
};
#endif