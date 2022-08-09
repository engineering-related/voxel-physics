#ifndef TEXTURE_2D
#define TEXTURE_2D

#include "Math.h"
#include "Texture.h"
#include <glad/glad.h>
#include <stb/stb_image.h>

namespace engine {
class Texture2D : public Texture {
public:
  Texture2D() { glGenTextures(1, &m_ID); }

  ~Texture2D() { glDeleteTextures(1, &m_ID); }

  void loadImage(const char *image, GLuint slot, TextureFormat format,
                 StorageType type) {
    // Assigns the type of the texture ot the texture object

    // Stores the width, height, and the number of color channels of the image
    int numColCh;
    // Flips the image so it appears right side up
    stbi_set_flip_vertically_on_load(true);
    // Reads the image from a file and stores it in bytes
    unsigned char *bytes =
        stbi_load(image, &m_Width, &m_Height, &numColCh, STBI_rgb_alpha);
    assert(bytes && "Could not load image.");
    // Assigns the texture to a Texture Unit
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    // Configures the type of algorithm that is used to make the image smaller
    // or bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Configures the way the texture repeats (if it does at all)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Assigns the image to the OpenGL Texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, (GLint)format,
                 (GLint)type, bytes);
    // Generates MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void setImage(int levels, TextureFormat internalFormat, uvec2 size,
                int border, TextureFormat format, StorageType type,
                const void *pixels = nullptr) {
    glTexImage2D(GL_TEXTURE_2D, levels, (GLint)internalFormat, size.x, size.y,
                 border, (GLint)format, (GLint)type, pixels);
  }

  inline GLenum asGLType() const override { return GL_TEXTURE_2D; }
};
} // namespace engine

#endif // !TEXTURE_2D