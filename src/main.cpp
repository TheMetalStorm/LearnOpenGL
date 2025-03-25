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
#include "FPSCam.h"
#include "Model.h"
#include "DirLight.h"
#include "SpotLight.h"
#include "PointLight.h"

unsigned int loadTexture(const char* path);


// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

FPSCam *camera = new FPSCam(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);
float dirLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
float dirLightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
float dirLightSpecular[3] = { .3f, .3f, .3f };

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
float shininess = 32.0f;


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

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		DEBUG = !DEBUG;

	}

	if (DEBUG) return;

	camera->moveFast = (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_REPEAT);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->ProcessKeyboard(RIGHT, deltaTime);
	
	


}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (DEBUG) 
	{
		lastX = xpos;
		lastY = ypos;
		return;
	}

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

	camera->ProcessMouseMovement(xoffset, yoffset);

	
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	if (DEBUG) return;

	camera->ProcessMouseScroll(yoffset);
}


int main(int argc, char* argv[]) {

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



	Shader lightingShader = Shader("Shader/BlinnPhong.vert", "Shader/BlinnPhong.frag");
	Shader pointLightShader = Shader("Shader/LightCube.vert", "Shader/LightCube.frag");

	Model backpack = Model("Resources/backpack/backpack.obj");
	Model cube = Model("Resources/cube.obj");

	DirLight dirLight;
	SpotLight spotLight;
	PointLight pointLights[std::size(pointLightPositions)];

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
		lightingShader.setVec3("viewPos", camera->Position);
		lightingShader.setFloat("material.shininess", shininess);

		dirLight.dir = glm::vec3(sin(glfwGetTime()), dirLight.dir.y, cos(glfwGetTime())),
			dirLight.ambient = glm::make_vec3(dirLightAmbient);
		dirLight.diffuse = glm::make_vec3(dirLightDiffuse); ;
		dirLight.specular = glm::make_vec3(dirLightSpecular);
		dirLight.sendToShader(lightingShader);

		spotLight.position = camera->Position;
		spotLight.direction = camera->Front;
		spotLight.ambient = glm::make_vec3(spotLightAmbient);
		spotLight.diffuse = glm::make_vec3(spotLightDiffuse);
		spotLight.specular = glm::make_vec3(spotLightSpecular);
		spotLight.sendToShader(lightingShader);

		for (int i = 0; i < std::size(pointLightPositions); i++) {

			pointLights[i].position = pointLightPositions[i];
			pointLights[i].ambient = glm::make_vec3(pointLightAmbient);
			pointLights[i].specular = glm::make_vec3(pointLightSpecular);
			pointLights[i].diffuse = glm::make_vec3(pointLightDiffuse);
			pointLights[i].sendToShader(lightingShader, i);
		}

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera->GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// model Transformation
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); 
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
		lightingShader.setMat4("model", model);

		// render the loaded model
		backpack.Draw(lightingShader);
		
		pointLightShader.use();
		pointLightShader.setMat4("projection", projection);
		pointLightShader.setMat4("view", view);


		for (int i = 0; i < std::size(pointLightPositions); i++) {

			pointLightShader.setVec3("light.ambient", glm::make_vec3(pointLightAmbient));
			pointLightShader.setVec3("light.specular", glm::make_vec3(pointLightSpecular));
			pointLightShader.setVec3("light.diffuse", glm::make_vec3(pointLightDiffuse));
			model = glm::mat4(1.0f);
			model = glm::translate(model, pointLights[i].position);
			model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
			pointLightShader.setMat4("model", model);
			cube.Draw(pointLightShader);
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