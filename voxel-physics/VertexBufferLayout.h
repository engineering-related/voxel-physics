#ifndef VERTEX_BUFFER_LAYOUT
#define VERTEX_BUFFER_LAYOUT

#include "Types.h"
#include <cassert>
#include <glad/glad.h>
#include <vector>

namespace engine {
struct VertexBufferElement {
  unsigned int type;
  unsigned int count;
  unsigned char normalized;

  static unsigned int getSizeOfType(unsigned int type) {
    switch (type) {
    case GL_FLOAT:
      return 4;
    case GL_UNSIGNED_INT:
      return 4;
    case GL_UNSIGNED_BYTE:
      return 1;
    }
    assert(false);
    return 0;
  }
};

class VertexBufferLayout {
public:
  template <typename T> void push(unsigned int count = 1) {
    // if constexpr (std::is_same_v<T, Vertex>)
    // {
    //   push<float>(3);
    //   push<float>(1);
    //   push<float>(3);
    // }
    if constexpr (std::is_same_v<T, float>) {
      m_VertexBufferElements.push_back({GL_FLOAT, count, GL_FALSE});
      m_Stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
    } else if constexpr (std::is_same_v<T, int>) {
      m_VertexBufferElements.push_back({GL_INT, count, GL_FALSE});
      m_Stride += count * VertexBufferElement::getSizeOfType(GL_INT);
    } else if constexpr (std::is_same_v<T, unsigned int>) {
      m_VertexBufferElements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
      m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT);
    } else if constexpr (std::is_same_v<T, unsigned char>) {
      m_VertexBufferElements.push_back({GL_UNSIGNED_BYTE, count, GL_TRUE});
      m_Stride += count * VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE);
    } else
      assert(false);
  }

  inline unsigned int getStride() const { return m_Stride; }

  inline const std::vector<VertexBufferElement> &
  getVertexBufferElements() const {
    return m_VertexBufferElements;
  }

  VertexBufferLayout() : m_Stride(0) {}

private:
  std::vector<VertexBufferElement> m_VertexBufferElements{};
  unsigned int m_Stride;
};
} // namespace engine

#endif // !VERTEX_BUFFER_LAYOUT
