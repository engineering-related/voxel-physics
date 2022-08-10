#include "Voxel.h"

namespace engine {

Voxel::Voxel(vec3 position, vec3 rotation, vec3 color) 
	: m_Position(position), m_Rotation(rotation), m_Color(color)
{
	s_Colors[s_InstanceCount] = m_Color;
	s_ModelMatrices[s_InstanceCount] = getMatrix();

	glBindBuffer(GL_ARRAY_BUFFER, s_ColorInstanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, s_InstanceCount * sizeof(vec3), sizeof(vec3) * 1, &s_Colors[s_InstanceCount]);

	glBindBuffer(GL_ARRAY_BUFFER, s_ModelInstanceVBO);
	glBufferSubData(GL_ARRAY_BUFFER, s_InstanceCount * sizeof(mat4x4), sizeof(mat4x4) * 1, &s_ModelMatrices[s_InstanceCount]);

	m_InstanceIndex = s_InstanceCount++;
}
Voxel::~Voxel()
{

}

void Voxel::init(const uint32_t& numInstances)
{
	s_Colors = new vec3[numInstances];
	s_ModelMatrices = new mat4x4[numInstances];

	// store instance color data in array buffer
    glGenBuffers(1, &s_ColorInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_ColorInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3) * numInstances, &s_Colors[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // store instance model data in array buffer
    glGenBuffers(1, &s_ModelInstanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, s_ModelInstanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mat4x4) * numInstances, &s_ModelMatrices[0], GL_STATIC_DRAW);
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

	// Vertex
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(3 * sizeof(float)));

	// Color (instance data)
	glBindBuffer(GL_ARRAY_BUFFER, s_ColorInstanceVBO);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	glVertexAttribDivisor(2, 1);

	// Model (instance data)
	glBindBuffer(GL_ARRAY_BUFFER, s_ModelInstanceVBO);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);
    glEnableVertexAttribArray(6);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(0));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 4));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 8));
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 16, (void*)(sizeof(float) * 12));

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);
}

void Voxel::end()
{
	glDeleteBuffers(1, &s_ModelInstanceVBO);
	glDeleteBuffers(1, &s_ColorInstanceVBO);
	glDeleteBuffers(1, &s_VoxelVBO);
	glDeleteBuffers(1, &s_VoxelEBO);
	glDeleteVertexArrays(1, &s_VoxelVAO);
	delete[] s_ModelMatrices;
	delete[] s_Colors;
}

void Voxel::draw(Shader* shader, Camera* camera, PointLight* light)
{
	shader->use();
	shader->setUniform<mat4x4>("u_View", camera->getViewMatrix());
	shader->setUniform<mat4x4>("u_Projection", camera->getProjectionMatrix());
	shader->setUniform<vec3>("u_CamPos", camera->getEye());
	shader->setUniform<vec3>("u_LightDir", light->getDirection());
	shader->setUniform<float>("u_Ambient", light->getAmbient());
	shader->setUniform<float>("u_Specular", light->getSpecular());

	glBindVertexArray(s_VoxelVAO);
	glDrawElementsInstanced(GL_TRIANGLES, s_Indices.size(), GL_UNSIGNED_INT, nullptr, s_InstanceCount);
}

mat4x4 Voxel::getMatrix() const 
{
	const mat4 transformX = rotate(mat4(1.0f),
		radians(m_Rotation.x),
		vec3(1.0f, 0.0f, 0.0f));
	const mat4 transformY = rotate(mat4(1.0f),
		radians(m_Rotation.y),
		vec3(0.0f, 1.0f, 0.0f));
	const mat4 transformZ = rotate(mat4(1.0f),
		radians(m_Rotation.z),
		vec3(0.0f, 0.0f, 1.0f));

	mat4 TRS(1.f);
	// translation * rotation * scale (also know as TRS matrix)
	TRS = translate(TRS, m_Position) * transformY * transformX * transformZ;
	return TRS;
}

}

