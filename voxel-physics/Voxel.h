#ifndef VOXEL
#define VOXEL

#include "Math.h"
#include "Shader.h"
#include "Camera.h"
#include "PointLight.h"
#include <array>

namespace engine {

struct Vertex {
  vec3 position;
  uint32_t face;
};

class Voxel
{
public:
	Voxel(vec3 position, vec3 rotation, vec3 color);
	~Voxel();
	static void init(const uint32_t& numInstances);
	static void drawAll(Shader* shader, Camera* camera, PointLight* light);
	static void end();

	inline const vec3& getPosition() const { return m_Position; }
	inline const vec3& getRotation() const { return m_Rotation; }
	inline const vec3& getColor() const { return m_Color; }

	void setPosition(const vec3& position);
	void setRotation(const vec3& eulerRotationDeg);
	void setColor(const vec3& color);

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
	inline static const std::array<Vertex, 24> s_Vertices = {
		// Front
		Vertex{{-1.0f/2, -1.0f/2, -1.0f/2}, 0},
		Vertex{{-1.0f/2, +1.0f/2, -1.0f/2}, 0},
		Vertex{{+1.0f/2, +1.0f/2, -1.0f/2}, 0},
		Vertex{{+1.0f/2, -1.0f/2, -1.0f/2}, 0},
		// Back
		Vertex{{-1.0f/2, -1.0f/2, +1.0f/2}, 1},
		Vertex{{-1.0f/2, +1.0f/2, +1.0f/2}, 1},
		Vertex{{+1.0f/2, +1.0f/2, +1.0f/2}, 1},
		Vertex{{+1.0f/2, -1.0f/2, +1.0f/2}, 1},
		// Top
		Vertex{{-1.0f/2, +1.0f/2, -1.0f/2}, 2},
		Vertex{{-1.0f/2, +1.0f/2, +1.0f/2}, 2},
		Vertex{{+1.0f/2, +1.0f/2, +1.0f/2}, 2},
		Vertex{{+1.0f/2, +1.0f/2, -1.0f/2}, 2},
		//  Bottom
		Vertex{{-1.0f/2, -1.0f/2, -1.0f/2}, 3},
		Vertex{{-1.0f/2, -1.0f/2, +1.0f/2}, 3},
		Vertex{{+1.0f/2, -1.0f/2, +1.0f/2}, 3},
		Vertex{{+1.0f/2, -1.0f/2, -1.0f/2}, 3},
		// Left
		Vertex{{-1.0f/2, -1.0f/2, -1.0f/2}, 4},
		Vertex{{-1.0f/2, -1.0f/2, +1.0f/2}, 4},
		Vertex{{-1.0f/2, +1.0f/2, +1.0f/2}, 4},
		Vertex{{-1.0f/2, +1.0f/2, -1.0f/2}, 4},
		// Right
		Vertex{{+1.0f/2, -1.0f/2, -1.0f/2}, 5},
		Vertex{{+1.0f/2, -1.0f/2, +1.0f/2}, 5},
		Vertex{{+1.0f/2, +1.0f/2, +1.0f/2}, 5},
		Vertex{{+1.0f/2, +1.0f/2, -1.0f/2}, 5}
	};

	inline static uint32_t s_InstanceCount = 0;
	inline static vec3* s_Colors;
	inline static mat4x4* s_ModelMatrices;
	inline static GLuint s_VoxelVAO, s_VoxelVBO, s_VoxelEBO;
	inline static GLuint s_ModelInstanceVBO, s_ColorInstanceVBO;

	void updateModelMatrixCPU();
	void updateColorCPU();
	static void updateModelMatrixGPU();
	static void updateColorGPU();
	
	mat4x4 getMatrix() const;

	vec3 m_Position;
	vec3 m_Rotation;
	vec3 m_Color;
	uint32_t m_InstanceIndex;
};

}


#endif // !VOXEL
