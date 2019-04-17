#version 430 core
out vec4 FragColour;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_normal1;

uniform vec3 lightPos;
uniform vec3 lightColour;
uniform vec3 viewPos;

void main()
{
	vec3 normal = texture(texture_normal1, TexCoords).rgb;
	
	vec3 normalvec = normalize(normal * 2.0 - 1.0);
	
	vec3 colour = texture(texture_diffuse1, TexCoords).rgb;
	//ambient light calulation
	float ambientStrength = 0.5;
	vec3 ambient = ambientStrength * lightColour;
	
	//diffuse lighting calulation
	vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
	float difference = max(dot(lightDir, normalvec), 0.0);
	
	vec3 diffuse = difference * lightColour;
	
	//specular lighting calulation
	vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
	vec3 reflectDir = reflect(-lightDir, normalvec);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normalvec, halfwayDir), 0.0), 8.0);
	float specularStrength = 0.3;
	vec3 specular = specularStrength * spec * lightColour;
	
	vec3 result = (ambient + diffuse + specular) * colour;
	
	FragColour = vec4(result, 1.0);
}