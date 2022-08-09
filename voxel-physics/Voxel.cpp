#include "Voxel.h"

namespace engine {

Voxel::Voxel(vec3 position, vec3 rotation, vec3 color)
{
	m_Position = position;
	m_Rotation = rotation;
	m_Color = color;

	const std::array<Vertex, 8> vertices = {
		Vertex{{ CUBE_POSITION.x - CUBE_SIZE.x / 2.f, CUBE_POSITION.y - CUBE_SIZE.y / 2.f, CUBE_POSITION.z + CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x - CUBE_SIZE.x / 2.f, CUBE_POSITION.y - CUBE_SIZE.y / 2.f, CUBE_POSITION.z - CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x + CUBE_SIZE.x / 2.f, CUBE_POSITION.y - CUBE_SIZE.y / 2.f, CUBE_POSITION.z - CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x + CUBE_SIZE.x / 2.f, CUBE_POSITION.y - CUBE_SIZE.y / 2.f, CUBE_POSITION.z + CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x - CUBE_SIZE.x / 2.f, CUBE_POSITION.y + CUBE_SIZE.y / 2.f, CUBE_POSITION.z + CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x - CUBE_SIZE.x / 2.f, CUBE_POSITION.y + CUBE_SIZE.y / 2.f, CUBE_POSITION.z - CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x + CUBE_SIZE.x / 2.f, CUBE_POSITION.y + CUBE_SIZE.y / 2.f, CUBE_POSITION.z - CUBE_SIZE.z / 2.f }, color },
		Vertex{{ CUBE_POSITION.x + CUBE_SIZE.x / 2.f, CUBE_POSITION.y + CUBE_SIZE.y / 2.f, CUBE_POSITION.z + CUBE_SIZE.z / 2.f }, color }
	};


	m_VAO = new VertexArray();
	m_VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
	m_EBO = new IndexBuffer(s_Indices.data(), s_Indices.size());

    VertexBufferLayout layout;
    layout.push<float>(3);
    layout.push<float>(3);
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
	TRS = translate(TRS, m_Position) * scale(TRS, CUBE_SIZE) * transformY * transformX * transformZ;
	return TRS;
}

void Voxel::draw(Shader* shader, Camera* camera) {
	shader->use();
	shader->setUniform<mat4x4>("u_Model", getMatrix());
	shader->setUniform<mat4x4>("u_CameraMatrix", camera->getMatrix());

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

