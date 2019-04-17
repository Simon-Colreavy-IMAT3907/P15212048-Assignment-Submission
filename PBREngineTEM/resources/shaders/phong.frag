#version 430
//Now this shader creates a blinn phong lighting setup
in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColour;

/*TODO:: Complete your shader code for a full Phong shading*/ //Week--5

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;

uniform sampler2D texture_diffuse1;

//Week--5: complete to a full phong shading

void main() {

   //Calculate the light vector
   vec3 colour = texture(texture_diffuse1, TexCoords).rgb;
    // ambient
  
    vec3 ambient = 0.5 * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColour;
    
    // specular
    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColour;  
        
    vec3 result = (ambient + diffuse + specular) * colour;
    FragColour = vec4(result, 1.0);
	//FragColour = texture(texture_diffuse1, TexCoords);

}
