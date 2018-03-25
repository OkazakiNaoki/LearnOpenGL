#include "header/GLSLloader.h"
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

GLSLloader::GLSLloader(const char* vertexGlslPath, const char* fragGlslPath)
{
	std::ifstream vertShaderFile;
	std::ifstream fragShaderFile;
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertShaderFile.open(vertexGlslPath);
		std::stringstream vertShaderStream;
		vertShaderStream << vertShaderFile.rdbuf();
		vertGlslCode = vertShaderStream.str();
		//std::cout << vertGlslCode << std::endl;
		fragShaderFile.open(fragGlslPath);
		std::stringstream fragShaderStream;
		fragShaderStream << fragShaderFile.rdbuf();
		fragGlslCode = fragShaderStream.str();
		//std::cout << fragGlslCode << std::endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	vertShaderCode = vertGlslCode.c_str();
	fragShaderCode = fragGlslCode.c_str();
	useGeometry = false;
}
GLSLloader::GLSLloader(const char* vertexGlslPath, const char* fragGlslPath, const char* geoGLSLPath)
{
	std::ifstream vertShaderFile;
	std::ifstream fragShaderFile;
	std::ifstream geoShaderFile;
	vertShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	geoShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		vertShaderFile.open(vertexGlslPath);
		std::stringstream vertShaderStream;
		vertShaderStream << vertShaderFile.rdbuf();
		vertGlslCode = vertShaderStream.str();
		//std::cout << vertGlslCode << std::endl;
		fragShaderFile.open(fragGlslPath);
		std::stringstream fragShaderStream;
		fragShaderStream << fragShaderFile.rdbuf();
		fragGlslCode = fragShaderStream.str();
		//std::cout << fragGlslCode << std::endl;
		geoShaderFile.open(geoGLSLPath);
		std::stringstream geoShaderStream;
		geoShaderStream << geoShaderFile.rdbuf();
		geoGlslCode = geoShaderStream.str();
		//std::cout << geoGlslCode << std::endl;
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	vertShaderCode = vertGlslCode.c_str();
	fragShaderCode = fragGlslCode.c_str();
	geoShaderCode = geoGlslCode.c_str();
	useGeometry = true;
}
const GLint* GLSLloader::GetLength(std::string glslCode)
{
	const GLint length = glslCode.size();
	return &length;
}
void GLSLloader::CreateCompile() {
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertShaderCode, GetLength(vertGlslCode));
	glCompileShader(vertexShader);
	std::cout << "-----Vertex Shader-----" << std::endl;
	PrintCompileError(vertexShader);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragShaderCode, GetLength(fragGlslCode));
	glCompileShader(fragmentShader);
	std::cout << "-----Fragment Shader-----" << std::endl;
	PrintCompileError(fragmentShader);
	if(useGeometry){
		geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometryShader, 1, &geoShaderCode, GetLength(geoGlslCode));
		glCompileShader(geometryShader);
		std::cout << "-----Geometry Shader-----" << std::endl;
		PrintCompileError(geometryShader);
	}
}
void GLSLloader::CreateProgram() {
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	if(useGeometry)
		glAttachShader(shaderProgram, geometryShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (useGeometry)
		glDeleteShader(geometryShader);
}
void GLSLloader::UseProgram() {
	glUseProgram(shaderProgram);
}
void GLSLloader::SetUniform1i(const std::string &uniformName, unsigned int uint1) {
	glUniform1i(glGetUniformLocation(shaderProgram, uniformName.c_str()), uint1);
}
void GLSLloader::SetUniform1f(const std::string &uniformName, float float1) {
	glUniform1f(glGetUniformLocation(shaderProgram, uniformName.c_str()), float1);
}
void GLSLloader::SetUniform2f(const std::string &uniformName, glm::vec2 float2) {
	glUniform2f(glGetUniformLocation(shaderProgram, uniformName.c_str()), float2.x, float2.y);
}
void GLSLloader::SetUniform3f(const std::string &uniformName, glm::vec3 float3) {
	glUniform3f(glGetUniformLocation(shaderProgram, uniformName.c_str()), float3.x, float3.y, float3.z);
}
void GLSLloader::SetUniform4f(const std::string &uniformName, glm::vec4 float4) {
	glUniform4f(glGetUniformLocation(shaderProgram, uniformName.c_str()), float4.x, float4.y, float4.z, float4.w);
}
void GLSLloader::SetUniformMat4(const std::string &uniformName, glm::mat4 matrix){
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, uniformName.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}
void GLSLloader::PrintCompileError(unsigned int shader) {
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << infoLog << std::endl;
	}
}
unsigned int GLSLloader::GetProgram() {
	return shaderProgram;
}