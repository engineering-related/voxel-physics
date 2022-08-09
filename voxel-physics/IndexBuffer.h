#ifndef INDEX_BUFFER
#define INDEX_BUFFER

#include "Types.h"
#include "VertexBuffer.h"
#include <cassert>
#include <glad/glad.h>
#include <unordered_map>

namespace engine {
class IndexBuffer {
public:
  IndexBuffer(const uint32_t *data, uint32_t count,
              BufferType type = BufferType::STATIC)
      : m_Count(count), m_Type(type) {
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data,
                 (GLenum)type);
  }

  ~IndexBuffer() { glDeleteBuffers(1, &m_ID); }

  void update(const uint32_t *data, uint32_t count) {
    assert(m_Type != BufferType::STATIC &&
           "Should not update static IndexBuffer with ID: " + m_ID);

    m_Count = count;

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
  }

  uint32_t getCount() { return m_Count; }

  void bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID); }

  void unbind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

private:
  GLuint m_ID;
  uint32_t m_Count;
  BufferType m_Type;
};
} // namespace engine

#endif // !INDEX_BUFFER