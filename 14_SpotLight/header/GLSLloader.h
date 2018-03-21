#ifndef GLSLLOADER_H
#define GLSLLOADER_H
#include <glad/glad.h>
#include <glm.hpp>
#include <string>

class GLSLloader
{
public:
	GLSLloader(const char* vertexGlslPath, const char* fragGlslPath);
	const GLint* GetLength(std::string glslCode);
	void CreateCompile();
	void CreateProgram();
	void UseProgram();
	void SetUniform1i(const std::string &uniformName, unsigned int uint1);
	void SetUniform1f(const std::string &uniformName, float float1);
	void SetUniform3f(const std::string &uniformName, glm::vec3 float3);
	void SetUniform4f(const std::string &uniformName, glm::vec4 float4);
	void SetUniformMat4(const std::string &uniformName, glm::mat4 matrix);
	void PrintCompileError(unsigned int shader);
private:
	std::string vertGlslCode;
	std::string fragGlslCode;
	const GLchar* vertShaderCode;
	const GLchar* fragShaderCode;
	unsigned int vertexShader;
	unsigned int fragmentShader;
	unsigned int shaderProgram;
};
#endif