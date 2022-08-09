#version 460 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the current position from the Vertex Shader
in vec3 Position;
// Imports the normal from the Vertex Shader
//in vec3 Normal;
// Imports the color from the Vertex Shader
in vec4 Color;

//uniform vec3 u_CamPos;

/* vec4 directLight() { */
/*   // Light direction vector */
/*   vec3 lightVec = vec3(0.5f, -1.0f, 0.0f); */
/**/
/*   // Ambient lighting */
/*   float ambient = 0.20f; */
/**/
/*   // Diffuse lighting  */
/*   vec3 normal = normalize(Normal); */
/*   vec3 lightDirection = normalize(lightVec*-1.f); */
/*   float diffuse = max(dot(normal, lightDirection), 0.0f); */
/**/
/*   // Specular lighting */
/*   float specularLight = 0.2f; */
/*   vec3 viewDirection = normalize(u_CamPos - Position); */
/*   vec3 reflectionDirection = reflect(-lightDirection, normal); */
/*   float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16); */
/*   float specular = specAmount * specularLight; */
/**/
/*   vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f); */
/*   return (diffuse + ambient + specular) * lightColor; */
/* } */

void main() {

  // Need to use a cubemap for textures
	FragColor = Color;
}