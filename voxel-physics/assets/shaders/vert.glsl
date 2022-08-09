#version 460 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals (not necessarily normalized)
/* layout (location = 1) in vec3 aNormal; */
// Colors
layout (location = 1) in vec4 aColor;
//layout (location = 3) in uint aPaletteIndex;

// Outputs the current position for the Fragment Shader
out vec3 Position;
// Outputs the normal for the Fragment Shader
//out vec3 Normal;
// Outputs the color for the Fragment Shader
out vec4 Color;

/* // Imports the model matrix from the main function */
uniform mat4 u_Model;
uniform mat4 u_CameraMatrix;

void main() {
  // calculates current position
  Position = vec3(u_Model * vec4(aPos, 1.0f));
  // Assigns the normal from the Vertex Data to "Normal"
  //Normal = aNormal;
  // Assigns the colors from the Vertex Data to "color"
  Color = aColor;
  // Assigns the texture coordinates from the Vertex Data to "texCoord"
  /* texCoord = aTex; */

  // Outputs the positions/coordinates of all vertices
  gl_Position = u_CameraMatrix * vec4(Position, 1.0);
}