#include "header/GLSLloader.h"
#include <fstream>
#include <sstream>
#include <iostream>

GLSLloader::GLSLloader(const char* glslPath)
{
	std::ifstream shaderFile;
	// ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shaderFile.open(glslPath);
		std::stringstream shaderStream;
		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		glslCode = shaderStream.str();
		std::cout << glslCode << std::endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	shaderCode = glslCode.c_str();
}

const GLint* GLSLloader::GetLength()
{
	const GLint length = glslCode.size();
	return &length;
}