#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "header/GLSLloader.h"
#include "header/TexLoader.h"
#include "header/ModelLoader.h"
#include "header/CustomKeyState.h"
#include <iostream>
#include <vector>

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;

const char *vertexShaderSource = "shader/vertex.glsl";
const char *fragmentShaderSource = "shader/fragment.glsl";

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0 / 2.0;
float fov = 60.0f;

CustomKeyState customKeyState;
float debugFlag1 = 1.0f;
float debugFlag2 = 1.0f;
float debugFlag3 = 1.0f;

void OnOff(float* flag) {
	if (*flag > 0)
		*flag = 0.0f;
	else
		*flag = 1.0f;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) // Keyboard key input detection
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_1))
		OnOff(&debugFlag1);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_2))
		OnOff(&debugFlag2);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_3))
		OnOff(&debugFlag3);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 120.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 120.0f)
		fov = 120.0f;
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Blend", NULL, NULL);
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

	glViewport(0, 0, screenWidth, screenHeight);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);
	//*********************************************
	//	Enable blend and set source alpha & (1 - source alpha)
	//*********************************************
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	customKeyState.RegisterKey(GLFW_KEY_KP_1);
	customKeyState.RegisterKey(GLFW_KEY_KP_2);
	customKeyState.RegisterKey(GLFW_KEY_KP_3);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  4.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 4.0f,

		5.0f, -0.5f,  5.0f,  4.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 4.0f,
		5.0f, -0.5f, -5.0f,  4.0f, 4.0f
	};

	float transparentVertices[] = {
		// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	GLSLloader planeShader("shader/vertexNoNorm.glsl", "shader/fragment.glsl");
	planeShader.CreateCompile();
	planeShader.CreateProgram();
	GLSLloader glassShader("shader/vertexNoNorm.glsl", "shader/fragTransparent.glsl");
	glassShader.CreateCompile();
	glassShader.CreateProgram();

	TexLoader floorTex("texture/dirt_tiled.png", GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST);
	unsigned int floorTexID = floorTex.GetMapID();
	GLint texLoc1 = glGetUniformLocation(planeShader.GetProgram(), "texture_diffuse1");
	glUniform1i(texLoc1, 0);
	TexLoader glassTex("texture/blending_transparent_window.png", GL_CLAMP_TO_EDGE, GL_LINEAR);
	unsigned int glassTexID = glassTex.GetMapID();
	GLint texLoc2 = glGetUniformLocation(glassShader.GetProgram(), "texture1");
	glUniform1i(texLoc2, 0);

	std::vector<glm::vec3> windows;
	windows.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	windows.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	windows.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	windows.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));
	windows.push_back(glm::vec3(0.5f, 0.0f, -0.6f));

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		//*********************************************
		//	Need dynamically calculate the order (since camera will move)
		//*********************************************
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(cameraPos - windows[i]);
			sorted[distance] = windows[i];
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexID);
		planeShader.UseProgram();
		glBindVertexArray(planeVAO);
		planeShader.SetUniformMat4("projection", projection);
		planeShader.SetUniformMat4("view", view);
		planeShader.SetUniformMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, glassTexID);
		glassShader.UseProgram();
		glassShader.SetUniformMat4("projection", projection);
		glassShader.SetUniformMat4("view", view);
		//*********************************************
		//	Not yet sort, from far to near
		//*********************************************
		/*for (unsigned int i = 0; i < vegetation.size(); i++)
		{
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, vegetation[i]);
			glassShader.SetUniformMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}*/
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			glm::mat4 model = glm::mat4();
			model = glm::translate(model, it->second);
			glassShader.SetUniformMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		// Event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glfwTerminate();
	return 0;
}