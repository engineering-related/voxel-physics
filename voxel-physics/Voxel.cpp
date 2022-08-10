#include "Voxel.h"

namespace engine {

void Voxels::init(const uint32_t& numInstances)
{
	s_PosOffsets = new vec3[numInstances];

    // store instance data in an array buffer
    glGenBuffers(1, &s_InstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_InstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * numInstances, &s_PosOffsets[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Create voxel buffers
	glGenVertexArrays(1, &s_VoxelVAO);
    glGenBuffers(1, &s_VoxelVBO);
	glGenBuffers(1, &s_VoxelEBO);

    glBindVertexArray(s_VoxelVAO);

    glBindBuffer(GL_ARRAY_BUFFER, s_VoxelVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * s_Vertices.size(), s_Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_VoxelEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * s_Indices.size(), s_Indices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	//also set instance data
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, s_InstanceVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.
}
void Voxels::create(vec3 position, vec3 rotation, vec3 color)
{
	s_PosOffsets[s_InstanceCount] = position;
	glBindBuffer(GL_ARRAY_BUFFER, s_InstanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, s_InstanceCount * sizeof(vec3), sizeof(vec3) * 1, &s_PosOffsets[s_InstanceCount]);
	s_InstanceCount++;
}

void Voxels::end()
{
	glDeleteBuffers(1, &s_InstanceVBO);
	glDeleteBuffers(1, &s_VoxelVBO);
	glDeleteBuffers(1, &s_VoxelEBO);
	glDeleteVertexArrays(1, &s_VoxelVAO);
	delete[] s_PosOffsets;
}

void Voxels::draw(Shader* shader, Camera* camera) {
	shader->use();
	glBindVertexArray(s_VoxelVAO);
	glDrawElementsInstanced(GL_TRIANGLES, s_Indices.size(), GL_UNSIGNED_INT, nullptr, s_InstanceCount);
}

}

