#version 430

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 lightPosView;
in vec3 reflectionNormal;
in vec3 reflectionPosition;
/*TODO:: Complete your shader code for a full Phong shading*/ //Week--5

//uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColour;
uniform vec3 objectColour;

uniform sampler2D texture_diffuse1;

uniform samplerCube skybox;


//Week--5: complete to a full phong shading
layout( location = 0 ) out vec4 FragColour;

void main() {

//Calculates the reflection
   vec3 incidentAngle = normalize(reflectionPosition - viewPos);
   vec3 reflectionAngle = reflect(incidentAngle, normalize(reflectionNormal));

   vec3 reflectionEquation = vec3(texture(skybox, reflectionAngle).rgb);

    //Calculate the light vector
    vec3 colour = texture(texture_diffuse1, TexCoords).rgb;
    // ambient
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColour;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPosView - FragPos);
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
	FragColour = vec4(mix(result, reflectionEquation, 0.5),1.0);
	//FragColour = vec4(result, 1.0);

}