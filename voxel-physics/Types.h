#ifndef TYPES
#define TYPES

#include <bitset>
#include <glad/glad.h>
#include <map>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <typeindex>

namespace engine {
using GLid = GLuint;
enum BufferBit { COLOR = GL_COLOR_BUFFER_BIT, DEPTH = GL_DEPTH_BUFFER_BIT };

// Input
enum class InputButtons { W, A, S, D, Q, E };
} // namespace engine

#endif // !TYPES
