#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) // Resize the viewport by changing GLFWwindow
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) // Keyboard key input detection
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

const char *vertexShaderSource =
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char *fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Rectangle", NULL, NULL);
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

	while (!glfwWindowShouldClose(window))
	{
		// Input
		processInput(window);
		// Render command
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//*********************************************
		//	6 Vertices position data array (overlap 2 vertices)
		//*********************************************
		float verticesOverlapped[] = {
			// first triangle
			0.5f,  0.5f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f,  0.5f, 0.0f,  // top left 
			// second triangle
			0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f, -0.5f, 0.0f,  // bottom left
			-0.5f,  0.5f, 0.0f   // top left
		};
		//*********************************************
		//	4 Vertices position data array
		//*********************************************
		float vertices[] = {
			0.5f,  0.5f, 0.0f,  // top right
			0.5f, -0.5f, 0.0f,  // bottom right
			-0.5f,  0.5f, 0.0f,  // top left
			-0.5f, -0.5f, 0.0f,  // bottom left
		};
		//*********************************************
		//	Indices for the vertices above
		//*********************************************
		unsigned int indices[] = {  // note that we start from 0!
			0, 1, 2,   // first triangle
			1, 3, 2    // second triangle
		};
		//*********************************************
		//	Create a Vertex Buffer Object(VBO)
		//*********************************************
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		//*********************************************
		//	Create a Element Buffer Object(EBO)
		//*********************************************
		unsigned int EBO;
		glGenBuffers(1, &EBO);
		//*********************************************
		//	Create a Vertex Array Object(VAO) -> Bind VAO
		//*********************************************
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		//*********************************************
		//	Bind VBO -> Feed data
		//*********************************************
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//*********************************************
		//	Bind EBO -> Feed data
		//*********************************************
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//*********************************************
		//	Set vertex attributes pointer
		//*********************************************
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // vertex position attribute
		//*********************************************
		//	Create a shader(vertex)
		//*********************************************
		unsigned int vertexShader;
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);
		//*********************************************
		//	Shader(vertex) compilation error check
		//*********************************************
		int  success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		//*********************************************
		//	Create a shader(fragment)
		//*********************************************
		unsigned int fragmentShader;
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);
		//*********************************************
		//	Create shader program -> Link compiled shader to shader program
		//*********************************************
		unsigned int shaderProgram;
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		//*********************************************
		//	Linking shader error check
		//*********************************************
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
		}
		//*********************************************
		//	Use shader program -> draw -> delete shader
		//*********************************************
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO); // Bind what we want to draw
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		// Event
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}