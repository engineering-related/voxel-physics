#ifndef EASING
#define EASING

#include "Math.h"

namespace engine
{
	enum class EaseFunction { LINEAR, IN_QUAD, OUT_QUAD, IN_OUT_QUAD, OUT_CUBIC, OUT_QUART, OUT_QUINT, IN_EXPO, OUT_EXPO, OUT_CIRC, IN_SINE, OUT_SINE };

	template <class T>
	class Ease
	{
	public:
		Ease(T start, T target, float duration, EaseFunction function = EaseFunction::LINEAR)
			: m_T(0), m_Target(target), m_Start(start), m_Duration(duration), m_Current(start), m_Function(linear)
		{	
			switch (function)
			{
			case EaseFunction::LINEAR:
				m_Function = linear;
				break;
			case EaseFunction::IN_QUAD:
				m_Function = inQuad;
				break;
			case EaseFunction::OUT_QUAD:
				m_Function = outQuad;
				break;
			case EaseFunction::IN_OUT_QUAD:
				m_Function = inOutQuad;
				break;
			case EaseFunction::OUT_CUBIC:
				m_Function = outCubic;
				break;
			case EaseFunction::OUT_QUART:
				m_Function = outQuart;
				break;
			case EaseFunction::OUT_QUINT:
				m_Function = outQuint;
				break;
			case EaseFunction::IN_EXPO:
				m_Function = inExpo;
				break;
			case EaseFunction::OUT_EXPO:
				m_Function = outExpo;
				break;
			case EaseFunction::OUT_CIRC:
				m_Function = outCirc;
				break;
			case EaseFunction::IN_SINE:
				m_Function = inSine;
				break;
			case EaseFunction::OUT_SINE:
				m_Function = outSine;
				break;
			default:
				assert(false && "VERIFY NOT REACHED");
				break;
			}
		}

		Ease() = default;

		void target(T newTarget)
		{
			m_Start = m_Current;
			m_Target = newTarget;
			m_T = 0;
		}

		const T& step(const float& t)
		{
			if (!targetReached())
			{
				m_T += t;
				m_Current = lerp(m_Start, m_Target, m_Function(m_T / m_Duration));
			}
			return m_Current;
		}

		const T& targetStep(T newTarget, const float& t)
		{
			target(newTarget);
			return step(t);
		}


		float percentCompleted()
		{
			return m_T / m_Duration;
		}

		bool targetReached()
		{
			return (m_T >= m_Duration);
		}

		static T lerp(T start_value, T end_value, float pct)
		{
			return (start_value + (end_value - start_value) * pct);
		}

		static float linear(float x)
		{
			return x;
		}

		static float inQuad(float x)
		{
			return x * x;
		}

		static float outQuad(float x)
		{
			return 1 - (1 - x) * (1 - x);
		}

		static float inOutQuad(float x)
		{
			return x < 0.5 ? 2 * x * x : 1 - pow(-2 * x + 2, 2) / 2;
		}

		static float outCubic(float x)
		{
			return 1 - pow(1 - x, 3);
		}

		static float outQuart(float x)
		{
			return 1 - pow(1 - x, 4);
		}

		static float outQuint(float x)
		{
			return 1 - pow(1 - x, 5);
		}
		static float inExpo(float x)
		{
			return x == 0 ? 0 : pow(2, 10 * x - 10);
		}
		static float outExpo(float x)
		{
			return x == 1 ? 1 : 1 - pow(2, -10 * x);
		}
		static float outCirc(float x)
		{
			return sqrt(1 - pow(x - 1, 2));
		}
		static float outSine(float x)
		{
			return sin((x * PI) / 2);
		}
		static float inSine(float x)
		{
			return 1 - cos((x * PI) / 2);
		}

	private:
		// https://easings.net/
	
		float(*m_Function)(float);

		T m_Target;
		T m_Start;
		T m_Current;
		float m_T;
		float m_Duration;
	};
}

#endif // !EASING