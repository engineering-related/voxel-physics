#ifndef SHADER
#define SHADER

#include "Math.h"
#include "Texture.h"
#include "Types.h"
#include "Utils.h"
#include <cassert>
#include <glad/glad.h>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>

namespace engine {

class Shader {
public:
  Shader(const std::string &vertexFile, const std::string &fragmentFile) {
    m_VertexFile = vertexFile;
    m_FragmentFile = fragmentFile;

    std::string vertexCode = getFileContents(vertexFile);
    std::string fragmentCode = getFileContents(fragmentFile);

    const char *vertexSource = vertexCode.c_str();
    const char *fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShader);
    glAttachShader(m_ID, fragmentShader);
    glLinkProgram(m_ID);
    compileErrors(m_ID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
  }

  Shader(const std::string &vertexFile, const std::string &fragmentFile,
         const std::string &geometryFile)
      : Shader(vertexFile, fragmentFile) {
    m_GeometryFile = geometryFile;
    std::string geometryCode = getFileContents(geometryFile);
    const char *geometrySource = geometryCode.c_str();

    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometrySource, NULL);
    glCompileShader(geometryShader);
    compileErrors(geometryShader, "GEOMETRY");

    glAttachShader(m_ID, geometryShader);
    glLinkProgram(m_ID);

    compileErrors(m_ID, "PROGRAM");
    glDeleteShader(geometryShader);
  }

  Shader(const std::string &computeFile) {
    m_ComputeFile = computeFile;
    std::string computeCode = getFileContents(computeFile);
    const char *computeSource = computeCode.c_str();

    GLuint computeShader = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShader, 1, &computeSource, NULL);
    glCompileShader(computeShader);
    compileErrors(computeShader, "COMPUTE");

    m_ID = glCreateProgram();
    glAttachShader(m_ID, computeShader);
    glLinkProgram(m_ID);

    compileErrors(m_ID, "PROGRAM");
    glDeleteShader(computeShader);
  }

  ~Shader() { glDeleteProgram(m_ID); }

  void use() const { glUseProgram(m_ID); }
  void compute(const uvec2 &screenRes,
               const uvec3 &workGroupsLayoutSize) const {
    assert(isComputeShader() &&
           "Can not call this function without provided compute file.");
    glDispatchCompute(ceil(screenRes.x / workGroupsLayoutSize.x),
                      ceil(screenRes.y / workGroupsLayoutSize.y),
                      workGroupsLayoutSize.z);
    glMemoryBarrier(GL_ALL_BARRIER_BITS);
  }

  void unUse() const { glUseProgram(0); }

  inline const bool isComputeShader() const { return !m_ComputeFile.empty(); }

  template <typename T> void setUniform(const char *name, const T &value) {
    if constexpr (std::is_same_v<T, int>) {
      glUniform1i(getUniformLocation(name), value);
    } else if constexpr (std::is_same_v<T, unsigned int>) {
      glUniform1ui(getUniformLocation(name), value);
    } else if constexpr (std::is_same_v<T, float>) {
      glUniform1f(getUniformLocation(name), value);
    } else if constexpr (std::is_same_v<T, fvec2>) {
      glUniform2f(getUniformLocation(name), value.x, value.y);
    } else if constexpr (std::is_same_v<T, fvec3>) {
      glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
    } else if constexpr (std::is_same_v<T, fvec4>) {
      glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
    } else if constexpr (std::is_same_v<T, mat4x4>) {
      glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
                         value_ptr(value));
    } else {
      LOG_FATAL << typeid(T).name();
      assert(!"Type is not implemented.");
    }
  }

  void bindTexture(std::shared_ptr<Texture> texture, int slot) {
    glActiveTexture(GL_TEXTURE0 + slot);
    texture->bind();
  }

  void bindImageTexture(std::shared_ptr<Texture> texture, const int &unit,
                        const TextureFormat &format, const Access &access,
                        const int &level = 0, const bool &layered = false,
                        const int &layer = 0) {
    glBindImageTexture(unit, texture->getID(), level, layered, layer,
                       (GLenum)access, (GLint)format);
  }

private:
  void compileErrors(GLuint glShader, const char *type) {
    GLint hasCompiled;
    char infoLog[1024];
    if (std::string(type) != "PROGRAM") {
      glGetShaderiv(glShader, GL_COMPILE_STATUS, &hasCompiled);
      if (hasCompiled == GL_FALSE) {
        glGetShaderInfoLog(glShader, 1024, NULL, infoLog);
        std::cout << "SHADER_COMPILATION_ERROR for: " << type << std::endl;
        std::cout << infoLog << std::endl;
      }
    } else {
      glGetProgramiv(glShader, GL_COMPILE_STATUS, &hasCompiled);
      if (hasCompiled == GL_FALSE) {
        glGetProgramInfoLog(glShader, 1024, NULL, infoLog);
        std::cout << "SHADER_LINKING_ERROR for: " << type << std::endl;
        std::cout << infoLog << std::endl;
      }
    }
  }

  int getUniformLocation(const char *name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
      return m_UniformLocationCache[name];

    int location = glGetUniformLocation(m_ID, name);

    if (location == -1) {
      std::cout << "Warning: Uniform " << name
                << " is not being used or does not exist for " << m_VertexFile
                << "/" << m_FragmentFile << std::endl;
    }
    m_UniformLocationCache[name] = location;

    return location;
  }

  GLuint m_ID;
  std::unordered_map<std::string, int> m_UniformLocationCache{};

  std::string m_VertexFile;
  std::string m_FragmentFile;
  std::string m_GeometryFile;
  std::string m_ComputeFile;
};
} // namespace engine
#endif // !SHADER
