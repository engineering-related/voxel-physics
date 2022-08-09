#ifndef VOXEL
#define VOXEL

#include "Math.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include <array>

namespace engine {

struct Vertex {
  vec3 position;
  vec3 color;
  uint32_t face;
};

class Voxel
{
public:
	Voxel(vec3 position, vec3 rotation, vec3 color);
	~Voxel();

	void draw(Shader* shader, Camera* camera);
	inline void setRotation(const vec3& eulerAngleDeg) { m_Rotation = eulerAngleDeg;  }
	inline void setPosition(const vec3& positionWorldSpace) { m_Position = positionWorldSpace; }

	inline const vec3& getRotation() const { return m_Rotation; }
	inline const vec3& getPosition() const { return m_Position; }

private:
	mat4x4 getMatrix() const;

	inline static const std::array<uint32_t, 36> s_Indices = {
	  // Front
	  0, 1, 2,
	  0, 2, 3,
	  // Back
	  4, 5, 6,
	  4, 6, 7,
	  // Top
	  8, 9, 10,
	  8, 10, 11,
	  // Bottom
	  12, 13, 14,
	  12, 14, 15,
	  // Left
	  16, 17, 18,
	  16, 18, 19,
	  // Right
	  20, 21, 22,
	  20, 22, 23
	};

	vec3 m_Position;
	vec3 m_Rotation;
	vec3 m_Color;

	VertexArray* m_VAO;
	VertexBuffer* m_VBO;
	IndexBuffer* m_EBO;
};

}


#endif // !VOXEL
