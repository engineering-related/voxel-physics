#include "Voxel.h"

namespace engine {

Voxel::Voxel(vec3 position, vec3 rotation, vec3 color)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Color = color;

	float w = 1.0f;

	const std::array<Vertex, 24> vertices = {
		// Front
		Vertex{{-w/2, -w/2, -w/2}, color, 0},
		Vertex{{-w/2, +w/2, -w/2}, color, 0},
		Vertex{{+w/2, +w/2, -w/2}, color, 0},
		Vertex{{+w/2, -w/2, -w/2}, color, 0},
		// Back
		Vertex{{-w/2, -w/2, +w/2}, color, 1},
		Vertex{{-w/2, +w/2, +w/2}, color, 1},
		Vertex{{+w/2, +w/2, +w/2}, color, 1},
		Vertex{{+w/2, -w/2, +w/2}, color, 1},
		// Top
		Vertex{{-w/2, +w/2, -w/2}, color, 2},
		Vertex{{-w/2, +w/2, +w/2}, color, 2},
		Vertex{{+w/2, +w/2, +w/2}, color, 2},
		Vertex{{+w/2, +w/2, -w/2}, color, 2},
		//  Bottom
		Vertex{{-w/2, -w/2, -w/2}, color, 3},
		Vertex{{-w/2, -w/2, +w/2}, color, 3},
		Vertex{{+w/2, -w/2, +w/2}, color, 3},
		Vertex{{+w/2, -w/2, -w/2}, color, 3},
		// Left
		Vertex{{-w/2, -w/2, -w/2}, color, 4},
		Vertex{{-w/2, -w/2, +w/2}, color, 4},
		Vertex{{-w/2, +w/2, +w/2}, color, 4},
		Vertex{{-w/2, +w/2, -w/2}, color, 4},
		// Right
		Vertex{{+w/2, -w/2, -w/2}, color, 5},
		Vertex{{+w/2, -w/2, +w/2}, color, 5},
		Vertex{{+w/2, +w/2, +w/2}, color, 5},
		Vertex{{+w/2, +w/2, -w/2}, color, 5}
	};


	m_VAO = new VertexArray();
	m_VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
	m_EBO = new IndexBuffer(s_Indices.data(), s_Indices.size());

    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(3);
	layout.push<float>(1);
    m_VAO->addVertexBuffer(m_VBO, layout);
}

mat4x4 Voxel::getMatrix() const {
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

void Voxel::draw(Shader* shader, Camera* camera) {
	shader->use();
	shader->setUniform<mat4x4>("u_Model", getMatrix());

	m_VAO->bind();
	glDrawElements(GL_TRIANGLES, s_Indices.size(), GL_UNSIGNED_INT, nullptr);
}

Voxel::~Voxel()
{
	delete m_EBO;
	delete m_VBO;
	delete m_VAO;
}

}

