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
  uint32_t face;
};

class Voxels
{
public:
	static void init(const uint32_t& numInstances);
	static void create(vec3 position, vec3 rotation, vec3 color);
	static void destroy();
	static void draw(Shader* shader, Camera* camera);
	static void end();

private:
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

	inline static const float w = 1.0f;
	inline static const std::array<Vertex, 24> s_Vertices = {
		// Front
		Vertex{{-w/2, -w/2, -w/2}, 0},
		Vertex{{-w/2, +w/2, -w/2}, 0},
		Vertex{{+w/2, +w/2, -w/2}, 0},
		Vertex{{+w/2, -w/2, -w/2}, 0},
		// Back
		Vertex{{-w/2, -w/2, +w/2}, 1},
		Vertex{{-w/2, +w/2, +w/2}, 1},
		Vertex{{+w/2, +w/2, +w/2}, 1},
		Vertex{{+w/2, -w/2, +w/2}, 1},
		// Top
		Vertex{{-w/2, +w/2, -w/2}, 2},
		Vertex{{-w/2, +w/2, +w/2}, 2},
		Vertex{{+w/2, +w/2, +w/2}, 2},
		Vertex{{+w/2, +w/2, -w/2}, 2},
		//  Bottom
		Vertex{{-w/2, -w/2, -w/2}, 3},
		Vertex{{-w/2, -w/2, +w/2}, 3},
		Vertex{{+w/2, -w/2, +w/2}, 3},
		Vertex{{+w/2, -w/2, -w/2}, 3},
		// Left
		Vertex{{-w/2, -w/2, -w/2}, 4},
		Vertex{{-w/2, -w/2, +w/2}, 4},
		Vertex{{-w/2, +w/2, +w/2}, 4},
		Vertex{{-w/2, +w/2, -w/2}, 4},
		// Right
		Vertex{{+w/2, -w/2, -w/2}, 5},
		Vertex{{+w/2, -w/2, +w/2}, 5},
		Vertex{{+w/2, +w/2, +w/2}, 5},
		Vertex{{+w/2, +w/2, -w/2}, 5}
	};

	inline static uint32_t s_InstanceCount = 0;
	inline static vec3* s_PosOffsets;
	inline static GLuint s_VoxelVAO, s_VoxelVBO, s_VoxelEBO;
	inline static GLuint s_InstanceVBO;
	virtual void makeAbstract() = 0;
};

}


#endif // !VOXEL
