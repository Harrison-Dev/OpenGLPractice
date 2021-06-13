#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "headers/shader.h"
#include "headers/stb_image.h"
#include "headers/camera.h"
#include "headers/model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);	
unsigned int loadTexture(const char *path);

void glProgramDebug(unsigned int shaderProgram);

void glShaderDebug(unsigned int vertexShader);

const int screenWidth = 800, screenHeight = 600;

float mixValue = 0.2f;
float fieldOfView = 45.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	Shader litObjShader("src/shader/vert.glsl", "src/shader/frag.glsl");
	
	Model testModel("models/nanosuit/nanosuit.obj");

	litObjShader.use();
	litObjShader.setInt("material.diffuse", 0);
	litObjShader.setInt("material.specular", 1);
	litObjShader.setInt("material.emission", 2);

	while (!glfwWindowShouldClose(window))
	{
		// Calculate delta time
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// activate shader
		litObjShader.use();

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // 降低影响
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

		litObjShader.setVec3("viewPos", camera.Position);

		// directional light
		litObjShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		litObjShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		litObjShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		litObjShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		litObjShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		litObjShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		litObjShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		litObjShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		litObjShader.setFloat("pointLights[0].constant", 1.0f);
		litObjShader.setFloat("pointLights[0].linear", 0.09);
		litObjShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		litObjShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		litObjShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		litObjShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		litObjShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		litObjShader.setFloat("pointLights[1].constant", 1.0f);
		litObjShader.setFloat("pointLights[1].linear", 0.09);
		litObjShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		litObjShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		litObjShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		litObjShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		litObjShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		litObjShader.setFloat("pointLights[2].constant", 1.0f);
		litObjShader.setFloat("pointLights[2].linear", 0.09);
		litObjShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		litObjShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		litObjShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		litObjShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		litObjShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		litObjShader.setFloat("pointLights[3].constant", 1.0f);
		litObjShader.setFloat("pointLights[3].linear", 0.09);
		litObjShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		litObjShader.setVec3("spotLight.position", camera.Position);
		litObjShader.setVec3("spotLight.direction", camera.Front);
		litObjShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		litObjShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		litObjShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		litObjShader.setFloat("spotLight.constant", 1.0f);
		litObjShader.setFloat("spotLight.linear", 0.09);
		litObjShader.setFloat("spotLight.quadratic", 0.032);
		litObjShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		litObjShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// material properties
		litObjShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		litObjShader.setFloat("material.shininess", 64.0f);

		float timeValue = glfwGetTime();
		float sineTime = sin(10 * timeValue);
		litObjShader.setFloat("sine_time", sineTime);
		litObjShader.setFloat("mix_t", mixValue);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		litObjShader.setMat4("projection", projection);
		litObjShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		litObjShader.setMat4("model", model);
		testModel.Draw(litObjShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void glProgramDebug(unsigned int shaderProgram)
{
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

void glShaderDebug(unsigned int vertexShader)
{
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
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