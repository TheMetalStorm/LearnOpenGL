#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

const char* vertexShaderSource = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n" // input of shader from vbo, different for every Vert
	"layout (location = 1) in vec3 aColor;\n" // input of shader from vbo, different for every Vert
	"out vec3 vertexColor;\n" // output of shader
	"void main()\n"
	"{\n"
	"   vertexColor = aColor;\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource= 
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"in vec3 vertexColor;\n" // input of shader 
	"void main()\n"
	"{\n"
	"   FragColor = vec4(vertexColor, 1.0);\n"
	"}\0";


unsigned int createFragShader(const char* shaderSource) {
	unsigned int  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderSource, NULL);
	glCompileShader(fragmentShader);
	if (!fragmentShader) {
		std::cout << "Failed to create fragment shader" << std::endl;
		exit(EXIT_FAILURE);
	}	return fragmentShader;
}

unsigned int createVertShader(const char* shaderSource) {
	unsigned int  vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &shaderSource, NULL);
	glCompileShader(vertexShader);
	if(!vertexShader) {
		std::cout << "Failed to create vertex shader" << std::endl;
		exit(EXIT_FAILURE);
	}
	return vertexShader;
}

int main() {

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};


	//VBO: vertex buffer object -> buffer of memory that gpu can access
	//VAO: vertex attribute object -> stores vertex bindings
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	//Shader setup
	unsigned int vertexShader = createVertShader(vertexShaderSource);
	unsigned int fragmentShader = createFragShader(fragmentShaderSource);
	

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	if (!shaderProgram) {
		std::cout << "Failed to create shader Program" << std::endl;
		exit(EXIT_FAILURE);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Bind VAO and specify vertex attributes
	glBindVertexArray(VAO);

	//Bind Buffers 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Set vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);

	//Set vertex attribute pointers
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//unbind vertex array
	glBindVertexArray(0);
	

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(.3f, 0.6f, .6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		
		glBindVertexArray(VAO); //explicitly bind the VAO
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Cleanup
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}