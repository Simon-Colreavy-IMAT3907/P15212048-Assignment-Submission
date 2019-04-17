#version 430

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 lightPosView; //Light position in eye coords
out vec3 reflectionNormal; //reflection normal
out vec3 reflectionPosition; //reflection position



uniform vec3 lightPos; // Light position 
uniform mat3 NormalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   reflectionNormal = mat3(transpose(inverse(model))) * aNormal;

   reflectionPosition = vec3(model * vec4(aPos, 1.0));

   FragPos = vec3(view * model * vec4(aPos,1.0)); 
     
   lightPosView = vec3(view * model * vec4(lightPos,1.0));  

   Normal = normalize(NormalMatrix * aNormal);
   
   TexCoords = aTexCoords;
      
   gl_Position = projection * view * model * vec4(aPos,1.0);
   
}
