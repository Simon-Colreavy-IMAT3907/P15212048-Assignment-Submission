#include "vboCube.h"
#include <string>
#include <iostream>
#include <stb_image.h>

VBOCube::VBOCube(float fvert, float CubeSizeIn, int shaderHandle)
	: skyboxSize(CubeSizeIn)
{
	/*CubeVertices = { -skyboxSize,-skyboxSize, -skyboxSize,		//V0
					-skyboxSize, skyboxSize, -skyboxSize,		//V1
					skyboxSize, skyboxSize, -skyboxSize,		//V2
					skyboxSize, -skyboxSize, -skyboxSize,		//V3
					-skyboxSize, -skyboxSize, skyboxSize		//V4
					-skyboxSize, skyboxSize, skyboxSize,		//V5
					skyboxSize, skyboxSize, skyboxSize,		    //V6
					skyboxSize, -skyboxSize, skyboxSize,		//V7
	};*/

	float CubeVertices[] =
	{
		-skyboxSize,-skyboxSize, -skyboxSize,		//V0
		-skyboxSize, skyboxSize, -skyboxSize,		//V1
		skyboxSize, skyboxSize, -skyboxSize,		//V2
		skyboxSize, -skyboxSize, -skyboxSize,		//V3
		-skyboxSize, -skyboxSize, skyboxSize,		//V4
		-skyboxSize, skyboxSize, skyboxSize,		//V5
		skyboxSize, skyboxSize, skyboxSize,		    //V6
		skyboxSize, -skyboxSize, skyboxSize,		//V7
	};


	GLuint Indices[] = {
		0,1,2,
		0,2,3,

		0,4,1,
		4,5,1,

		4,5,6,
		4,6,7,

		3,2,6,
		3,6,7,

		1,2,5,
		2,5,6,

		0,7,3,
		0,7,4
	};
	//GL_
	faces = sizeof(Indices) / 3;

	//vao&vbo handling

	GLuint handle[2];						//holds a vertice and a indice
	glGenBuffers(2, handle);				// gen vbo names

											//Gens a new vao and applies its address to the vaoHandle
	glGenVertexArrays(1, &vaoHandle);															// gen vao names
	glBindVertexArray(vaoHandle);																//Bind vao
	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);												//Binds the buffer handle to an array buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), &CubeVertices, GL_STATIC_DRAW);		//Fills the buffer with vertices
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, FALSE, 0, ((GLubyte*)NULL + (0)));			//Sets the attributes of the buffer
	glEnableVertexAttribArray(0);																//Binds the attributes to the current array
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[1]);										//Binds the indices buffer to an element array buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);		//Fills the buffer with indices
	glBindVertexArray(0);																		//The vertex array is then bound

//Texture
//Gens 2 new vbos to contain the cube data
	//texture reading and assighnmnets

	
	//faces
	GLuint CubeMapFaces[6] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,	//right cube face
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,	//left cube face
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,	//top cube face
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,	//bottom cube face
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,	//back cube face
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z		//front cube face
	};

	
	
	
		//textureLocation = "resources/textures/newCubeMap" + std::to_string(i) + ".jpg";
		p_FilePath.push_back("resources/textures/newCubeMap0.jpg");
		p_FilePath.push_back("resources/textures/newCubeMap1.jpg");
		p_FilePath.push_back("resources/textures/newCubeMap2.jpg");
		p_FilePath.push_back("resources/textures/newCubeMap3.jpg");
		p_FilePath.push_back("resources/textures/newCubeMap4.jpg");
		p_FilePath.push_back("resources/textures/newCubeMap5.jpg");

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int l_ImageWidth, l_ImageHeight, l_NrComponents;
	unsigned char* l_TextureData;

	for (GLuint i = 0; i < p_FilePath.size(); i++)
	{
		l_TextureData = stbi_load(p_FilePath[i], &l_ImageWidth, &l_ImageHeight, &l_NrComponents, 0);

		if (l_TextureData)
		{
			GLenum format;
			if (l_NrComponents == 1)
				format = GL_RED;
			else if (l_NrComponents == 3)
				format = GL_RGB;
			else if (l_NrComponents == 4)
				format = GL_RGBA;

			glTexImage2D(CubeMapFaces[i], 0, format, l_ImageWidth, l_ImageHeight, 0, format, GL_UNSIGNED_BYTE, l_TextureData);
		}
		else
		{
			std::cout << "Texture failed to load from: " << p_FilePath[i] << std::endl;
		}
		stbi_image_free(l_TextureData);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

void VBOCube::render() const
{
	glBindVertexArray(vaoHandle);		//Binds the VAO to be drawn
	//Draw the contents of the VAO as triangls using the indices
	glDrawElements(GL_TRIANGLES, 6 * faces, GL_UNSIGNED_INT, ((GLubyte*)NULL + (0)));
	glBindVertexArray(0);

}

int VBOCube::getTextureID()
{
	return textureID;
}