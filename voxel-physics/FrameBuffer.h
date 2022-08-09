#ifndef FRAMEBUFFER
#define FRAMEBUFFER

#include "RenderBuffer.h"
#include "Texture2D.h"
#include <glad/glad.h>
#include <memory>
#include <unordered_map>

namespace engine {
enum BufferBit { COLOR = GL_COLOR_BUFFER_BIT, DEPTH = GL_DEPTH_BUFFER_BIT };

class FrameBuffer {
public:
  enum class AttachmentType {
    COLOR0 = GL_COLOR_ATTACHMENT0,
    COLOR1 = GL_COLOR_ATTACHMENT1,
    COLOR2 = GL_COLOR_ATTACHMENT2,
    DEPTH = GL_DEPTH_ATTACHMENT
  };

  FrameBuffer() { glGenFramebuffers(1, &m_ID); }

  ~FrameBuffer() { glDeleteFramebuffers(1, &m_ID); }

  void attachTexture(const AttachmentType &toAttachment,
                     std::shared_ptr<Texture> texture, int level = 0) {
    m_Attachments[(uint32_t)toAttachment] = texture;
    glFramebufferTexture(GL_FRAMEBUFFER, (GLint)toAttachment, texture->getID(),
                         level);
  }

  void attachTexture2D(const AttachmentType &toAttachment,
                       std::shared_ptr<Texture2D> texture, int level = 0) {
    m_Attachments[(uint32_t)toAttachment] = texture;
    glFramebufferTexture2D(GL_FRAMEBUFFER, (GLint)toAttachment,
                           texture->asGLType(), texture->getID(), level);
  }

  std::shared_ptr<Texture> getTexture(const AttachmentType &fromAttachment) {
    return m_Attachments[(uint32_t)fromAttachment];
  }

  void attachRenderBuffer(const AttachmentType &attachment,
                          const RenderBuffer &renderBuffer) {
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, (GLint)attachment,
                              renderBuffer.asGLType(), renderBuffer.getID());
  }

  void
  setDrawBuffers(const std::vector<AttachmentType> attachmentTargets = {}) {
    std::vector<uint32_t> uVec;
    for (auto &attachmentTarget : attachmentTargets)
      uVec.push_back((uint32_t)attachmentTarget);

    if (uVec.size() == 0) {
      glDrawBuffer(GL_NONE);
      glReadBuffer(GL_NONE);
      return;
    }

    for (auto &attachment : uVec)
      assert(m_Attachments.find(attachment) != m_Attachments.end() &&
             "Warning: setting non existing attachment as draw-target.");

    glDrawBuffers(uVec.size(), uVec.data());
  }

  void checkStatus() {
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
      std::cout << status << std::endl;
      assert(false && "FrameBuffer not complete.");
    }
  }

  void bind() const { glBindFramebuffer(GL_FRAMEBUFFER, m_ID); }
  void unBind() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

  const GLuint &getID() const { return m_ID; }

private:
  GLuint m_ID;
  std::unordered_map<uint32_t, std::shared_ptr<Texture>> m_Attachments{};
};
} // namespace engine
#endif
