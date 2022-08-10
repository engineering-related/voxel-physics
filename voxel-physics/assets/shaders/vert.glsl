#version 460 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
layout (location = 1) in int aFace;
layout (location = 2) in vec3 aPosOffset;

// Outputs the current position for the Fragment Shader
out vec3 Position;
out vec3 Normal;
out vec4 Color;

// Imports the model matrix from the main function
uniform mat4 u_View;
uniform mat4 u_Projection;

vec3 getNormal(int face) {
  switch(face) {
    case 0: // Front
      return vec3(0.0f, 0.0f, -1.0f);
    case 1: // Back
      return vec3(0.0f, 0.0f, 1.0f);
    case 2: // Top
      return vec3(0.0f, 1.0f, 0.0f);
    case 3: // Bottom
      return vec3(0.0f, -1.0f, 0.0f);
    case 4: // Left
      return vec3(-1.0f, 0.0f, 0.0f);
    case 5: // Right
      return vec3(1.0f, 0.0f, 0.0f);
  }
}

void main() {
  Position = vec3(vec4(aPos + aPosOffset, 1.0f));
  //mat3 normalMatrix = transpose(inverse(mat3(aModel)));
  Normal = /*normalMatrix **/ getNormal(aFace);
  Color = vec4(1.0f);

  gl_Position = u_Projection * u_View * vec4(Position, 1.0);
}