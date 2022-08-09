#ifndef VERTEX_ARRAY
#define VERTEX_ARRAY

#include "Types.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <glad/glad.h>

namespace engine {
class VertexArray {
public:
  VertexArray() {
    glGenVertexArrays(1, &m_ID);
    glBindVertexArray(m_ID);
  }

  ~VertexArray() { glDeleteVertexArrays(1, &m_ID); }

  void bind() const { glBindVertexArray(m_ID); }

  void unbind() const { glBindVertexArray(0); }

  void addVertexBuffer(const VertexBuffer* vertexBuffer,
                       const VertexBufferLayout &vertexBufferLayout) {
    bind();
    vertexBuffer->bind();
    const auto &vertexBufferElements =
        vertexBufferLayout.getVertexBufferElements();
    uint32_t offset = 0;

    for (uint32_t i = 0; i < vertexBufferElements.size(); i++) {
      const auto &element = vertexBufferElements[i];
      glEnableVertexAttribArray(i);
      glVertexAttribPointer(i, element.count, element.type, element.normalized,
                            vertexBufferLayout.getStride(),
                            reinterpret_cast<const void*>(offset));
      offset +=
          element.count * VertexBufferElement::getSizeOfType(element.type);
    }
  }

private:
  GLuint m_ID;
};
} // namespace engine

#endif // !VERTEX_ARRAY
