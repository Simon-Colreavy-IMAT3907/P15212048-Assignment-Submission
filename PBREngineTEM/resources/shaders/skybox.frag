#version 430
//skybox fragment shader

in vec3 TexCoords;				//Loads in the texture coordinates from the vertex shader
out vec4 colour;				//outputs the colour
uniform samplerCube cubemap;	//leads the cube map from the main code

void main()
{
 //Sets the colour of the pixel being shaded to the coordinate colour on the cube map
  colour = texture(cubemap, TexCoords);
  //colour = vec4(1.0);
 
}