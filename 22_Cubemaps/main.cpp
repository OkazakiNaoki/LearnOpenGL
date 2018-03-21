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
int debugFlag1 = 1;
int debugFlag2 = 0;
int debugFlag3 = 0;
int debugFlag4 = 0;
int debugFlag5 = 0;
int debugFlag6 = 0;
int* debugFlags[6] = { &debugFlag1, &debugFlag2, &debugFlag3, &debugFlag4, &debugFlag5, &debugFlag6 };

void OnOff(float* flag) {
	if (*flag > 0)
		*flag = 0.0f;
	else
		*flag = 1.0f;
}

void OnOnlyOne(int* flag) {
	for (int i = 0; i < (sizeof(debugFlags) / sizeof(debugFlags[0])); i++)
		*debugFlags[i] = 0;
	*flag = 1;
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
		OnOnlyOne(&debugFlag1);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_2))
		OnOnlyOne(&debugFlag2);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_3))
		OnOnlyOne(&debugFlag3);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_4))
		OnOnlyOne(&debugFlag4);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_5))
		OnOnlyOne(&debugFlag5);
	if (customKeyState.GetKeyDown(window, GLFW_KEY_KP_6))
		OnOnlyOne(&debugFlag6);
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

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Cubemap", NULL, NULL);
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

	customKeyState.RegisterKey(GLFW_KEY_KP_1);
	customKeyState.RegisterKey(GLFW_KEY_KP_2);
	customKeyState.RegisterKey(GLFW_KEY_KP_3);
	customKeyState.RegisterKey(GLFW_KEY_KP_4);
	customKeyState.RegisterKey(GLFW_KEY_KP_5);
	customKeyState.RegisterKey(GLFW_KEY_KP_6);
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

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	std::vector<std::string> skyboxPath
	{
		"texture/cubemap/skybox2/right.jpg",
		"texture/cubemap/skybox2/left.jpg",
		"texture/cubemap/skybox2/top.jpg",
		"texture/cubemap/skybox2/bottom.jpg",
		"texture/cubemap/skybox2/front.jpg",
		"texture/cubemap/skybox2/back.jpg"
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

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	GLSLloader planeShader("shader/vertexNoNorm.glsl", "shader/fragment.glsl");
	planeShader.CreateCompile();
	planeShader.CreateProgram();
	GLSLloader cubeShader("shader/vertex.glsl", "shader/fragment.glsl");
	cubeShader.CreateCompile();
	cubeShader.CreateProgram();
	GLSLloader skyboxShader("shader/vertexSky.glsl", "shader/fragSky.glsl");
	skyboxShader.CreateCompile();
	skyboxShader.CreateProgram();

	TexLoader floorTex("texture/Tileable_marble.jpg", GL_REPEAT, GL_LINEAR, GL_LINEAR);
	unsigned int floorTexID = floorTex.GetMapID();
	GLint texLoc1 = glGetUniformLocation(planeShader.GetProgram(), "texture_diffuse1");
	glUniform1i(texLoc1, 0);

	TexLoader containerTex("texture/container.jpg", GL_REPEAT, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR);
	ModelLoader ourModel("model/cube/cube.obj", &containerTex, false);
	//ModelLoader ourModel("model/cube/cube.obj", false);

	TexLoader skyTex(skyboxPath, GL_LINEAR, GL_LINEAR);
	unsigned int skyTexID = skyTex.GetMapID();
	GLint texLoc2 = glGetUniformLocation(skyboxShader.GetProgram(), "skybox");
	glUniform1i(texLoc2, 0);
	
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexID);
		planeShader.UseProgram();
		glBindVertexArray(planeVAO);
		planeShader.SetUniformMat4("projection", projection);
		planeShader.SetUniformMat4("view", view);
		planeShader.SetUniformMat4("model", glm::mat4());
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		cubeShader.UseProgram();
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		cubeShader.SetUniformMat4("projection", projection);
		cubeShader.SetUniformMat4("view", view);
		cubeShader.SetUniformMat4("model", model);
		ourModel.Draw(cubeShader);

		//*********************************************
		//	Make the default framebuffer active
		//*********************************************
		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.UseProgram();
		view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp))); // remove translation from the view matrix
		skyboxShader.SetUniformMat4("view", view);
		skyboxShader.SetUniformMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default


		// Event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);
	glfwTerminate();
	return 0;
}