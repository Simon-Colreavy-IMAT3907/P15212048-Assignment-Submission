#version 430

layout (location = 0) in vec3 VertexPosition;
out vec3 TexCoords;
uniform mat4 VM;
uniform mat4 P;

void main()
{
  gl_Position = P * VM * vec4(VertexPosition, 1.0f);
  TexCoords = VertexPosition;
}