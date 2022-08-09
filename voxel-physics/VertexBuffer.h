#ifndef VERTEX_BUFFER
#define VERTEX_BUFFER

#include "Types.h"
#include <cassert>
#include <glad/glad.h>
#include <set>

namespace engine {
enum class BufferType {
  STATIC = GL_STATIC_DRAW,
  DYNAMIC = GL_DYNAMIC_DRAW,
  STREAM = GL_STREAM_DRAW
};

class VertexBuffer {
public:
  VertexBuffer(const void *data, unsigned int size,
               BufferType bufferType = BufferType::STATIC)
      : m_ID(0), m_Type(bufferType) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, size, data, (GLenum)bufferType);
  }

  ~VertexBuffer() { glDeleteBuffers(1, &m_ID); }

  void update(const void *data, unsigned int size) {
    assert(m_Type != BufferType::STATIC &&
           "Should not update static vertexbuffer");

    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
  }

  void bind() const { glBindBuffer(GL_ARRAY_BUFFER, m_ID); }
  void unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

private:
  GLuint m_ID;
  BufferType m_Type;
};
} // namespace engine

#endif // !VERTEX_BUFFER