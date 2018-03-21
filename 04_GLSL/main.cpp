#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "header/GLSLloader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // Resize the viewport by changing GLFWwindow
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) // Keyboard key input detection
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void PrintCompileError(unsigned int shader){
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

const char *vertexShaderSource = "shader/vertex.glsl";
const char *fragmentShaderSource = "shader/fragment.glsl";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "GLSL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//*********************************************
	//	Get maximum number of vertex attribute supported
	//*********************************************
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	float vertices[] = {
		// positions		// colors
		0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f    // top 
	};

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLSLloader vertCode(vertexShaderSource);
	glShaderSource(vertexShader, 1, &vertCode.shaderCode, vertCode.GetLength());
	glCompileShader(vertexShader);
	PrintCompileError(vertexShader);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLSLloader fragCode(fragmentShaderSource);
	glShaderSource(fragmentShader, 1, &fragCode.shaderCode, fragCode.GetLength());
	glCompileShader(fragmentShader);
	PrintCompileError(fragmentShader);

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		//*********************************************
		//	Set uniform attribute
		//*********************************************
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // Set frag shader uniform attribute
		//glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); // Require use program first
		glBindVertexArray(VAO); // Bind what we want to draw
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		// Event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}