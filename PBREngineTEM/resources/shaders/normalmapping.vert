#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
//out vec3 Normal;
out vec2 TexCoords;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	TexCoords = aTexCoords;
	
	mat3 NormalMatrix = transpose(inverse(mat3(model)));
	vec3 TangentNormal = normalize(NormalMatrix * aTangent);
	vec3 Normal = normalize(NormalMatrix * aNormal);
	TangentNormal = normalize(TangentNormal - dot(TangentNormal, Normal)* Normal);
	vec3 BiTangentNormal = normalize(aBitangent);
	
	mat3 TBN = transpose(mat3(TangentNormal, BiTangentNormal, Normal));
	TangentLightPos = TBN * lightPos;
	TangentViewPos = TBN * viewPos;
	TangentFragPos = TBN * FragPos;
	
	gl_Position = projection * view * vec4(aPos, 1.0f);
}