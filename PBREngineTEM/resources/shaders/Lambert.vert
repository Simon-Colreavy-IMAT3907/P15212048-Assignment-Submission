#version 430

out vec3 N;
out vec3 V;

void main()
{
	V = vec3(gl_ModelViewMatrix * gl_Vertex);
	N = normalize(gl_NormalMatrix * gl_Normal);
	
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}