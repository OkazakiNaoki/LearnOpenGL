#ifndef GLSLLOADER_H
#define GLSLLOADER_H
#include <glad/glad.h>
#include <string>

class GLSLloader
{
public:
	GLSLloader(const char* glslPath);
	const GLint* GetLength();
	const GLchar* shaderCode;
	std::string glslCode;
};
#endif