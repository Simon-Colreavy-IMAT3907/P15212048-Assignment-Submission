#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <STB_IMAGE/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

class VBOCube
{
public :
	VBOCube(float fvert, float CubeSizeIn, int shaderHandle);				//default constructor
	//std::vector<float>CubeVertices;
	void render() const;	// renders the cube
	int getTextureID(); //returns the Texture id

private :
	unsigned int vaoHandle;	// Holds the position of the VAO
	int faces;				//Contains the number of faces the shap has
	GLuint textureID;
	float skyboxSize;
	unsigned int vboHandle;
	std::vector<const char*> p_FilePath;
	std::string textureLocation;
};