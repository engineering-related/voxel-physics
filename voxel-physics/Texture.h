#ifndef TEXTURE
#define TEXTURE 

#include "Math.h"
#include <Logger.h>
#include <glad/glad.h>
#include <string.h>
#include <unordered_map>

namespace engine {
enum class TextureFormat {
  RGB = GL_RGB,
  RGBA = GL_RGBA,
  RGBA16F = GL_RGBA16F,
  RGBA32F = GL_RGBA32F,
  DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
  DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
  RED = GL_RED,
  R8 = GL_R8
};
enum class SamplerWrapping {
  CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  REPEAT = GL_REPEAT
};
enum class SamplerMipmap { LINEAR = GL_LINEAR, NEAREST = GL_NEAREST };
enum class StorageType { FLOAT = GL_FLOAT, U_BYTE = GL_UNSIGNED_BYTE };
enum class Access {READ_ONLY = GL_READ_ONLY, WRITE_ONLY = GL_WRITE_ONLY, READ_WRITE = GL_READ_WRITE };

class Texture {
public:
  const GLuint &getID() const { return m_ID; }

  virtual GLenum asGLType() const = 0;
  virtual ~Texture(){};

  void bind() const { glBindTexture(asGLType(), m_ID); }

  void unBind() const { glBindTexture(asGLType(), 0); }

  void setWrapping(const SamplerWrapping &wrapping) {
    glTexParameteri(asGLType(), GL_TEXTURE_WRAP_S, (GLint)wrapping);
    glTexParameteri(asGLType(), GL_TEXTURE_WRAP_T, (GLint)wrapping);
  }

  void setMinificationFilter(const SamplerMipmap &samplerMimpmap) {
    glTexParameteri(asGLType(), GL_TEXTURE_MIN_FILTER, (GLint)samplerMimpmap);
    glTexParameteri(asGLType(), GL_TEXTURE_MAG_FILTER, (GLint)samplerMimpmap);
  }

  void setBorderColor(const vec4 &color = {1.0f, 1.0f, 1.0f, 1.0f}) {
    float borderColor[] = {color.r, color.g, color.b, color.a};
    glTexParameterfv(asGLType(), GL_TEXTURE_BORDER_COLOR, borderColor);
  }

  inline const int &getWidth() { return m_Width; }
  inline const int &getHeight() { return m_Height; }

  // FIXME: This is a tmp fix, remove later
  inline static GLuint addImageUnitIndex(const std::string &name) {
    m_Units[name] = m_UnitIndex;
    LOG_VERBOSE << "Image-unit index: " << m_UnitIndex;
    return m_UnitIndex++;
  }
  inline static GLuint getImageUnitIndex(const std::string &name) {
    return m_Units[name];
  }

protected:
  int m_Width;
  int m_Height;
  GLuint m_ID;

private:
  inline static GLuint m_UnitIndex = 0;
  inline static std::unordered_map<std::string, GLuint> m_Units{};
};
} // namespace engine

#endif // !TEXTURE