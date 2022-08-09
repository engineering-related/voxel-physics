#version 460 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;
in vec4 Color;

uniform vec3 u_CamPos;
uniform vec3 u_LightDir;
uniform float u_Ambient;
uniform float u_Specular;

vec4 directLight() 
{
 // Ambient
 float ambient = u_Ambient;

 // Diffuse lighting
 vec3 normal = normalize(Normal);
 vec3 lightDirection = normalize(u_LightDir*-1.f);
 float diffuse = max(dot(normal, lightDirection), 0.0f);

 // Specular lighting
 vec3 viewDirection = normalize(u_CamPos - Position);
 vec3 reflectionDirection = reflect(-lightDirection, normal);
 float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
 float specular = specAmount * u_Specular;

 return (diffuse + ambient + specular) * vec4(1.f);
}

void main() 
{
	FragColor = directLight() * Color;
}