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
};

class Voxel
{
public:
	Voxel(vec3 position, vec3 rotation, vec3 color);
	~Voxel();

	void draw(Shader* shader, Camera* camera);

private:
	mat4x4 getMatrix() const;

	inline static const vec3 CUBE_POSITION = { 0, 0, 0 };
	inline static const vec3 CUBE_SIZE = { 1, 1, 1 };

	inline static const std::array<uint32_t, 36> s_Indices = {
	  0, 1, 2,
	  0, 2, 3,
	  0, 4, 7,
	  0, 7, 3,
	  3, 7, 6,
	  3, 6, 2,
	  2, 6, 5,
	  2, 5, 1,
	  1, 5, 4,
	  1, 4, 0,
	  4, 5, 6,
	  4, 6, 7
	};

	vec3 m_Position;
	vec3 m_Rotation;
	uvec3 m_Size;
	vec3 m_Color;

	VertexArray* m_VAO;
	VertexBuffer* m_VBO;
	IndexBuffer* m_EBO;
};

}


#endif // !VOXEL
