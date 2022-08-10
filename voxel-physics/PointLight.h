#ifndef LIGHT
#define LIGHT

#include "Math.h"

namespace engine 
{

class PointLight
{
public:
	PointLight(vec3 direction, float ambient, float specular) 
		: m_Direction(direction), m_Ambient(ambient), m_Specular(specular)
	{}

	inline vec3& getDirection() { return m_Direction; }
	inline float& getAmbient() { return m_Ambient; }
	inline float& getSpecular() { return m_Specular; }

private:
	vec3 m_Direction;
	float m_Ambient;
	float m_Specular;
};

}

#endif // !LIGHT
