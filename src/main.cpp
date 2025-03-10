#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Shader.h"
#include "Camera.h"

unsigned int loadTexture(const char* path);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
float dirLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
float dirLightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
float dirLightSpecular[3] = { 1.0f, 1.0f, 1.0f };

float spotLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
float spotLightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
float spotLightSpecular[3] = { 1.0f, 1.0f, 1.0f };

float pointLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
float pointLightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
float pointLightSpecular[3] = { 1.0f, 1.0f, 1.0f };


glm::vec3 pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

float specular[3] = { 0.5f, 0.5f, 0.5f };
float shininess = 64.0f;


float lightFlyRadius = 4.0f;
bool DEBUG = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		DEBUG = !DEBUG;

	if (DEBUG) return;

	camera.moveFast = (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_REPEAT);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
	


}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (DEBUG) return;

	if (firstMouse) // initially set to true
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);

	
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (DEBUG) return;

	camera.ProcessMouseScroll(yoffset);
}


int main(int argc, char * argv[]) {

	//Setup
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "My Title", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create window" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	gladLoadGL();
	glfwSwapInterval(1);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);


	// Callbacks
	glfwSetErrorCallback(error_callback);
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	//v, t, n
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,  1.0f,  0.0f
	};


	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	//VBO: vertex buffer object -> buffer of memory that gpu can access
	//VAO: vertex attribute object -> stores vertex bindings
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	//Bind VAO and specify vertex attributes
	glBindVertexArray(VAO);

	//Bind Buffers 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0);
	glEnableVertexAttribArray(0);


	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//unbind vertex array
	glBindVertexArray(0);



	// Light Source
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	//Bind VAO and specify vertex attributes
	glBindVertexArray(lightVAO);

	//Bind Buffers 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//unbind vertex array
	glBindVertexArray(0);


	Shader lightCubeShader = Shader("Shader/LightCube.vert", "Shader/LightCube.frag");
	Shader lightingShader = Shader("Shader/PhongShading.vert", "Shader/PhongShading.frag");

	lightingShader.use();
	lightingShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);

	
	unsigned int diffuseMap = loadTexture("Ressources/container2.png");
	unsigned int speculareMap = loadTexture("Ressources/container2_specular.png");

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, speculareMap);


	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);




	while (!glfwWindowShouldClose(window))
	{	
		if (DEBUG) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		
		glClearColor(.3f, 0.6f, .6f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		lightingShader.use();

		

		glm::mat4 model, view = glm::mat4(1.0);
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		
		//Transformations
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setVec3("viewPos", camera.Position);
		
		// Mat
		lightingShader.setFloat("material.shininess", shininess);


		// Directional Light
		lightingShader.setVec3("dirLight.ambient", dirLightAmbient[0], dirLightAmbient[1], dirLightAmbient[2]);
		lightingShader.setVec3("dirLight.diffuse", dirLightDiffuse[0], dirLightDiffuse[1], dirLightDiffuse[2]);
		lightingShader.setVec3("dirLight.specular", dirLightSpecular[0], dirLightSpecular[1], dirLightSpecular[2]);
		lightingShader.setVec3("dirLight.direction", camera.Front);


		// Point Lights

		for (int i = 0; i < sizeof(pointLightPositions)/sizeof(pointLightPositions[0]);i++) {

			char buffer[64];
			sprintf_s(buffer, "pointLight[%d].ambient", i);
			lightingShader.setVec3(buffer, pointLightAmbient[0], pointLightAmbient[1], pointLightAmbient[2]);
			sprintf_s(buffer, "pointLight[%d].diffuse", i);
			lightingShader.setVec3(buffer, pointLightDiffuse[0], pointLightDiffuse[1], pointLightDiffuse[2]);
			sprintf_s(buffer, "pointLight[%d].specular", i);
			lightingShader.setVec3(buffer, pointLightSpecular[0], pointLightSpecular[1], pointLightSpecular[2]);
			sprintf_s(buffer, "pointLight[%d].direction", i);
			lightingShader.setVec3(buffer, pointLightPositions[i]);

			sprintf_s(buffer, "pointLight[%d].constant", i);
			lightingShader.setFloat(buffer, 1.0f);
			sprintf_s(buffer, "pointLight[%d].linear", i);
			lightingShader.setFloat(buffer, 0.09f);
			sprintf_s(buffer, "pointLight[%d].quadratic", i);
			lightingShader.setFloat(buffer, 0.032f);
		}


		//SpotLight
		lightingShader.setVec3("spotLight.ambient", spotLightAmbient[0], spotLightAmbient[1], spotLightAmbient[2]);
		lightingShader.setVec3("spotLight.diffuse", spotLightDiffuse[0], spotLightDiffuse[1], spotLightDiffuse[2]);
		lightingShader.setVec3("spotLight.specular", spotLightSpecular[0], spotLightSpecular[1], spotLightSpecular[2]);
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//Set Outer Cutoff for Soft Light
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09f);
		lightingShader.setFloat("spotLight.quadratic", 0.032f);
		
		
		
		glBindVertexArray(VAO);

		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		lightCubeShader.use();
		for (int i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]);i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));

			lightCubeShader.setMat4("model", model);
			lightCubeShader.setMat4("projection", projection);
			lightCubeShader.setMat4("view", view);

			lightCubeShader.setVec3("light.ambient", pointLightAmbient[0], pointLightAmbient[1], pointLightAmbient[2]);
			lightCubeShader.setVec3("light.diffuse", pointLightDiffuse[0], pointLightDiffuse[1], pointLightDiffuse[2]);
			lightCubeShader.setVec3("light.specular", pointLightSpecular[0], pointLightSpecular[1], pointLightSpecular[2]);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		

		//ImGui setup
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//ImGui window
		ImGui::Begin("Light Controls");
			ImGui::InputFloat("Material Shininess", &shininess);
			ImGui::InputFloat3("DirLight Ambient", dirLightAmbient);
			ImGui::InputFloat3("DirLight Diffuse", dirLightDiffuse);
			ImGui::InputFloat3("DirLight Specular", dirLightSpecular);
			ImGui::InputFloat3("PointLight Ambient", pointLightAmbient);
			ImGui::InputFloat3("PointLight Diffuse", pointLightDiffuse);
			ImGui::InputFloat3("PointLight Specular", pointLightSpecular);
			ImGui::InputFloat3("SpotLight Ambient", spotLightAmbient);
			ImGui::InputFloat3("SpotLight Diffuse", spotLightDiffuse);
			ImGui::InputFloat3("SpotLight Specular", spotLightSpecular);
		ImGui::End();


		//ImGui draw
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();


	}

	//Cleanup
	

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}