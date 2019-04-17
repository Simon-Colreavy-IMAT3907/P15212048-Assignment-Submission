
#version 430 core

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoords;

out vec4 FragColour;
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;

uniform sampler2D texture_diffuse1;


void main()
{

	vec3 colour = texture(texture_diffuse1, TexCoords).rgb;
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    // specular
    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - FragPos);
	vec3 viewv = normalize(-FragPos);
	vec3 halfway = normalize(viewDir + viewv);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(norm, halfway), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColour;  
        
    vec3 result = (ambient + diffuse + specular) * colour;
    FragColour = vec4(result, 1.0);
	//FragColour = texture(texture_diffuse1, TexCoords);
} 