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
#include <random>

const unsigned int screenWidth = 800;
const unsigned int screenHeight = 600;
const unsigned int shadowWidth = 4096, shadowHeight = 4096;
bool lockedScreen = true;

bool isSphereGen = false;

CustomKeyState customKey;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = 800.0f / 2.0;
float lastY = 600.0f / 2.0;
float fov = 60.0f;
double mousePosX, mousePosY;

bool steelBall = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) // Keyboard key input detection
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	float cameraSpeed = 0.1f;
	if (customKey.GetMouseDown(window, GLFW_MOUSE_BUTTON_RIGHT)) {
		lockedScreen = !lockedScreen;
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (customKey.GetKeyDown(window, GLFW_KEY_KP_1)) {
		steelBall = !steelBall;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (!lockedScreen) {
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

unsigned int indexCount;
void renderSphere(unsigned int* sphereVAO, unsigned int* sphereVBO, unsigned int* sphereEBO)
{
	if (!isSphereGen)
	{
		glGenVertexArrays(1, sphereVAO);

		glGenBuffers(1, sphereVBO);
		glGenBuffers(1, sphereEBO);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
		{
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y)
		{
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x)
				{
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			}
			else
			{
				for (int x = X_SEGMENTS; x >= 0; --x)
				{
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i)
		{
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0)
			{
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0)
			{
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(*sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, *sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));

		isSphereGen = true;
	}

	glBindVertexArray(*sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}

int main()
{
	// enable vt100
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "PBR Lighting", NULL, NULL);
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

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	customKey.RegisterKey(GLFW_KEY_KP_1);
	customKey.RegisterKey(GLFW_MOUSE_BUTTON_RIGHT);

	// light data
	// ------
	glm::vec3 lightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3(10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3(10.0f, -10.0f, 10.0f),
	};
	glm::vec3 lightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;
	
	unsigned int sphereVAO, sphereVBO, sphereEBO;

	TexLoader albedoMap("texture/rustediron2_basecolor.png", GL_REPEAT, GL_LINEAR, GL_LINEAR, false);
	GLint albedoMapID = albedoMap.GetMapID();
	TexLoader normalMap("texture/rustediron2_normal.png", GL_REPEAT, GL_LINEAR, GL_LINEAR, false);
	GLint normalMapID = normalMap.GetMapID();
	TexLoader metallicMap("texture/rustediron2_metallic.png", GL_REPEAT, GL_LINEAR, GL_LINEAR, false);
	GLint metallicMapID = metallicMap.GetMapID();
	TexLoader roughnessMap("texture/rustediron2_roughness.png", GL_REPEAT, GL_LINEAR, GL_LINEAR, false);
	GLint roughnessMapID = roughnessMap.GetMapID();
	TexLoader aoMap("texture/white_ao.png", GL_REPEAT, GL_LINEAR, GL_LINEAR, false);
	GLint aoMapID = aoMap.GetMapID();

	GLSLloader pbrShader("shader/vertPBR.glsl", "shader/fragPBR.glsl");
	pbrShader.CreateCompile();
	pbrShader.CreateProgram();

	GLSLloader pbrTexShader("shader/vertPBR.glsl", "shader/fragPBRtex.glsl");
	pbrTexShader.CreateCompile();
	pbrTexShader.CreateProgram();

	pbrShader.UseProgram();
	pbrShader.SetUniform3f("albedo", glm::vec3(0.5f, 0.0f, 0.0f));
	pbrShader.SetUniform1f("ao", 1.0f);

	pbrTexShader.UseProgram();
	pbrTexShader.SetUniform1i("albedoMap", 0);
	pbrTexShader.SetUniform1i("normalMap", 1);
	pbrTexShader.SetUniform1i("metallicMap", 2);
	pbrTexShader.SetUniform1i("roughnessMap", 3);
	pbrTexShader.SetUniform1i("aoMap", 4);
	

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		float currentFrame = glfwGetTime();

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glm::mat4 projection = glm::perspective(glm::radians(fov), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glm::mat4 model;
		
		if (!steelBall) {
			pbrShader.UseProgram();
			pbrShader.SetUniformMat4("projection", projection);
			pbrShader.SetUniformMat4("view", view);
			pbrShader.SetUniform3f("camPos", cameraPos);

			// render rows*column number of spheres with varying metallic/roughness values scaled by rows and columns respectively
			for (int row = 0; row < nrRows; ++row)
			{
				pbrShader.SetUniform1f("metallic", (float)row / (float)nrRows);
				for (int col = 0; col < nrColumns; ++col)
				{
					// we clamp the roughness to 0.025 - 1.0 as perfectly smooth surfaces (roughness of 0.0) tend to look a bit off
					// on direct lighting.
					pbrShader.SetUniform1f("roughness", glm::clamp((float)col / (float)nrColumns, 0.05f, 1.0f));

					model = glm::mat4();
					model = glm::translate(model, glm::vec3(
						(col - (nrColumns / 2)) * spacing,
						(row - (nrRows / 2)) * spacing,
						0.0f
					));
					pbrShader.SetUniformMat4("model", model);
					renderSphere(&sphereVAO, &sphereVBO, &sphereEBO);
				}
			}
		}
		else {
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, albedoMapID);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, normalMapID);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, metallicMapID);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, roughnessMapID);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, aoMapID);
			pbrTexShader.UseProgram();
			pbrTexShader.SetUniformMat4("projection", projection);
			pbrTexShader.SetUniformMat4("view", view);
			pbrTexShader.SetUniform3f("camPos", cameraPos);
			for (int row = 0; row < nrRows; ++row)
			{
				for (int col = 0; col < nrColumns; ++col)
				{
					model = glm::mat4();
					model = glm::translate(model, glm::vec3(
						(float)(col - (nrColumns / 2)) * spacing,
						(float)(row - (nrRows / 2)) * spacing,
						0.0f
					));
					pbrTexShader.SetUniformMat4("model", model);
					renderSphere(&sphereVAO, &sphereVBO, &sphereEBO);
				}
			}
		}

		// render light source (simply re-render sphere at light positions)
		// this looks a bit off as we use the same shader, but it'll make their positions obvious and 
		// keeps the codeprint small.
		pbrShader.SetUniform3f("albedo", glm::vec3(1.0f, 1.0f, 1.0f));
		for (unsigned int i = 0; i < sizeof(lightPositions) / sizeof(lightPositions[0]); ++i)
		{
			glm::vec3 newPos = lightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0) * 5.0, 0.0, 0.0);
			newPos = lightPositions[i];
			pbrShader.SetUniform3f("lightPositions[" + std::to_string(i) + "]", newPos);
			pbrShader.SetUniform3f("lightColors[" + std::to_string(i) + "]", lightColors[i]);

			model = glm::mat4();
			model = glm::translate(model, newPos);
			model = glm::scale(model, glm::vec3(0.5f));
			pbrShader.SetUniformMat4("model", model);
			renderSphere(&sphereVAO, &sphereVBO, &sphereEBO);
		}
		pbrShader.SetUniform3f("albedo", glm::vec3(0.5f, 0.0f, 0.0f));

		std::cout << "\33[2K\r";

		// Event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &sphereVAO);
	glDeleteBuffers(1, &sphereVBO);
	glDeleteBuffers(1, &sphereEBO);
	glfwTerminate();
	return 0;
}