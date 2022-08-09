#ifndef MATH
#define MATH

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include "glm/fwd.hpp"
#include <glm/gtx/io.hpp>
#include <limits>
#include <random>

#define PI glm::pi<float>()

namespace engine {
  using namespace glm; 
  inline std::random_device dev;
	inline std::mt19937 rng(dev());
}

#endif // !MATH