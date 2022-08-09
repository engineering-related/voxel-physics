#ifndef RENDER_BUFFER
#define RENDER_BUFFER

#include <glad/glad.h>
#include "Math.h"

namespace engine {
enum class RenderBufferFormat {
  DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
  DEPTH_24_STENCIL_8 = GL_DEPTH24_STENCIL8
};

class RenderBuffer {
public:
  RenderBuffer() { glGenRenderbuffers(1, &m_ID); }

  ~RenderBuffer() { glDeleteRenderbuffers(1, &m_ID); }

  void setStorage(const RenderBufferFormat &format, const uvec2 &size) {
    bind();
    glRenderbufferStorage(GL_RENDERBUFFER, (GLint)format, size.x, size.y);
  }

  void bind() const { glBindRenderbuffer(GL_RENDERBUFFER, m_ID); }
  void unBind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

  const GLuint &getID() const { return m_ID; }
  const GLenum asGLType() const { return GL_RENDERBUFFER; }

private:
  GLuint m_ID;
};
} // namespace engine

#endif // !RENDER_BUFFER
