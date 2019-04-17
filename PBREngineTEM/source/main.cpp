#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB_IMAGE/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <camera.h>
#include <model.h>
#include <Primitives.h>
#include <vboCube.h>

#include <iostream>
#include <vector>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
void setParameters(Shader* p_Shader);
void setLightParameters(Shader* p_Shader);
enum changeShader{blinnphong, phong, reflection, refraction, normal, end};
changeShader changeShaderID;
bool currentShader;

// settings
unsigned int SCR_WIDTH = 1280;
unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

												
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PBREngineTEM", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, key_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{												 
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Global openGL states
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LEQUAL); // set depth function to less than AND equal for skybox depth trick.

	Shader* m_Shader = new Shader("resources/shaders/defaultShader.vert", "resources/shaders/defaultShader.frag");
	Shader* m_Shaderphong = new Shader("resources/shaders/phong.vert", "resources/shaders/phong.frag");
	Shader* m_ShaderReflection = new Shader("resources/shaders/Reflection.vert", "resources/shaders/Reflection.frag");
	Shader* m_ShaderRefraction = new Shader("resources/shaders/Refraction.vert", "resources/shaders/Refraction.frag");
	Shader* m_SkyboxShader = new Shader("resources/shaders/skybox.vert", "resources/shaders/skybox.frag");
	Shader* m_ShaderNormal = new Shader("resources/shaders/normalmapping.vert", "resources/shaders/normalmapping.frag");
	Primitives* m_Primitives = new Primitives();
	Model* m_Model = new Model("resources/models/nanosuit/nanosuit.obj");
	VBOCube* skyBox = new VBOCube(8, 50.0f, m_SkyboxShader->GetID());

	//glTexParameterf(GL_TEXTURE_CUBE_MAP)

	setLightParameters(m_Shader);
	setLightParameters(m_Shaderphong);
	setLightParameters(m_ShaderNormal);
	setLightParameters(m_ShaderReflection);
	setLightParameters(m_ShaderRefraction);
	setLightParameters(m_SkyboxShader);
	m_SkyboxShader->Use();
	m_SkyboxShader->SetInt("cubemap", 0);

	m_ShaderReflection->Use();
	m_ShaderReflection->SetInt("skybox", 5);

	m_ShaderRefraction->Use();
	m_ShaderRefraction->SetInt("skybox", 5);

	// render loop
	while (!glfwWindowShouldClose(window))
	{

		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// Render
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		setParameters(m_SkyboxShader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->getTextureID());
		skyBox->render();
		
		glEnable(GL_CULL_FACE);
		glm::mat4 model;
		switch (changeShaderID) 
		{

		case blinnphong :
			//Seperate into seperate functions/classes
			setParameters(m_Shader);
			model = glm::mat4(1.0f);
			m_Shader->SetMat4("model", model);
			m_Model->Render(m_Shader->GetID());
			break;
		case reflection :
			//Seperate into seperate functions/classes
			setParameters(m_ShaderReflection);
			model = glm::mat4(1.0f);
			m_ShaderReflection->SetMat4("model", model);
			m_ShaderReflection->Use();
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->getTextureID());
			m_Model->Render(m_ShaderReflection->GetID());
			break;
		case refraction :
			//Seperate into seperate functions/classes
			setParameters(m_ShaderRefraction);
			model = glm::mat4(1.0f);
			m_ShaderRefraction->SetMat4("model", model);
			m_ShaderRefraction->Use();
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyBox->getTextureID());
			m_Model->Render(m_ShaderRefraction->GetID());
			break;

		case phong :
			setParameters(m_Shaderphong);
			model = glm::mat4(1.0f);
			m_Shaderphong->SetMat4("model", model);
			m_Model->Render(m_Shaderphong->GetID());
			break;
		case normal :
			setParameters(m_ShaderNormal);
			model = glm::mat4(1.0f);
			m_ShaderNormal->SetMat4("model", model);
			m_Model->Render(m_ShaderNormal->GetID());
			break;
		}
		glDisable(GL_CULL_FACE);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float cameraSpeed = 2.5 * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		changeShaderID = static_cast<changeShader>(static_cast<int>(changeShaderID) + 1);
		if (changeShaderID == changeShader::end)
		{
			changeShaderID = changeShader::blinnphong;
		}
	}
}

void setParameters(Shader* p_Shader)
{
	p_Shader->Use();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 mv = camera.GetViewMatrix() * model;
	p_Shader->SetMat3("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(1.0f), glm::vec3(mv[2])));
	glm::mat4 m_projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	p_Shader->SetMat4("view", camera.GetViewMatrix());
	p_Shader->SetMat4("projection", m_projectionMatrix);
	p_Shader->SetVec3("viewPos", camera.Position);

	glm::mat4 viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));
	p_Shader->SetMat4("VM", viewMatrix);
	p_Shader->SetMat4("P", m_projectionMatrix);
}

void setLightParameters(Shader* p_Shader)
{
	p_Shader->Use();
	glm::vec3 worldLight = glm::vec3(-1.0f, 5.0f, 10.0f);
	p_Shader->SetVec3("objectColour", glm::vec3(0.46f, 0.29f, 0.0f));
	p_Shader->SetVec3("lightColour", glm::vec3(0.9f, 0.9f, 0.9f));
	p_Shader->SetVec3("lightPos", worldLight);
}